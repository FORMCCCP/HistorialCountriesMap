#include "sqlrepository.h"

#include <sqlite3.h>
#include <nlohmann/json.hpp>

#include <QDebug>

using nlohmann::json;

namespace {

std::string colText(sqlite3_stmt* stmt, int col){
    // 把SQL执行的语句获取的数据，取出并转换成
    const unsigned char* p = sqlite3_column_text(stmt, col);
    return p ? reinterpret_cast<const char*>(p) : std::string();
}

std::string jsonString(const json& obj, const char* key){
    // 取JSON对象里的字符串
    const auto it = obj.find(key);  // 根据键获取对象的迭代器
    if(it == obj.end() || !it->is_string()){
        return std::string();
    }
    return it->get<std::string>();  // 转化为string类
}

void parseDate(const std::string& s, int& year){
    // 把时间从字符串解析成年份
    // 空字符串默认为9999年(最高年)
    if(s.empty()){
        year = 9999;
        return;
    }
    year = std::stoi(s);
}

// 解析一个坐标环，外环/内环
std::vector<Point> parseRing(const json& ring){
    std::vector<Point> poly;

    for(const auto& r : ring){
        if(!r.is_array() || r.size() < 2) continue;

        poly.push_back(Point{r[0].get<double>(), r[1].get<double>()});
    }
    return poly;
}

// 解析一个带时间戳的环对象{start, end, coordinates}
Ring parseRingObjec(const json& obj){
    Ring ring;
    parseDate(jsonString(obj, "start"), ring.startYear);
    parseDate(jsonString(obj, "end"), ring.endYear);
    ring.points = parseRing(obj.at("coordinates"));
    return ring;
}

// 解析一个多边形(一个外环 + 若干个内环)
Polygon parsePolygonObject(const json& obj){
    Polygon poly;

    // 外环解析
    poly.out = parseRingObjec(obj);

    // 内环解析
    const json inRing = obj.value("inRing", json::array()); // 获取内环
    for(const auto &v : inRing){
        poly.holes.push_back(parseRingObjec(v));
    }
    return poly;
}

}



SQLRepository::SQLRepository() {}


bool SQLRepository::loadSQLcountries(const std::string& path){
    // 连接sqlite数据库并读取数据

    sqlite3* db = nullptr;
    const int openRc = sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READONLY, nullptr);

    if(openRc != SQLITE_OK){
        qDebug() << "无法打开数据库";
        if(db) sqlite3_close(db);   // 关闭数据库
        return false;
    }

    // 需要执行的SQL语句
    const char* sql =
        "SELECT id, name, color, start_date, end_date, description, geometry_json "
        "FROM countries ORDER BY id";

    sqlite3_stmt* stmt = nullptr;   // 一条已编译的 SQL 语句类型

    // 执行语句，
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        qDebug() << "查询表失败";
        sqlite3_close(db);
        return false;
    }

    m_regions.clear();      // 清空缓存区
    bool parseOk = true;    // 终止符

    while(parseOk){
        const int stepRc = sqlite3_step(stmt);
        if(stepRc == SQLITE_DONE)   // 读完退出
            break;

        if(stepRc != SQLITE_ROW){   // 读取错误
            qDebug() << "读取表失败";
            parseOk = false;
            break;
        }


        Region r;
        r.id = colText(stmt, 0);
        r.name = colText(stmt, 1);
        r.color = colText(stmt, 2);
        parseDate(colText(stmt, 3), r.startYear);
        parseDate(colText(stmt, 4), r.endYear);
        r.description = colText(stmt, 5);

        const std::string geomJson = colText(stmt, 6);
        const json gdoc = json::parse(geomJson);

        for(int i = 0; i<gdoc.size(); i++){
            Polygon poly = parsePolygonObject(gdoc[i]);
            r.polygons.push_back(poly);
        }

        r.prepareWorldProjection(); // 三角化处理
        qDebug() << QString::fromStdString(r.name) << " 加载成功";
        m_regions.push_back(std::move(r));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}


// 按时间筛选：返回活跃区域，并筛出该年下活跃的多边形及其活跃洞。
std::vector<Region> SQLRepository::regionsAt(int year) const{
    std::vector<Region> out;
    for(const Region& r : m_regions){
        if(!r.activeAt(year)) continue;

        Region copy = r;
        copy.polygons.clear();
        for(const Polygon &p : r.polygons){
            if(!p.out.activeAt(year)) continue;
            Polygon keep = p;
            keep.holes.clear();
            for(const Ring& h : p.holes){
                if(h.activeAt(year)){
                    keep.holes.push_back(h);
                }
            }
            copy.polygons.push_back(keep);
        }
        if(copy.polygons.empty()) continue;
        copy.prepareWorldProjection();
        out.push_back(copy);
    }
    return out;
}