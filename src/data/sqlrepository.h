#pragma once
#include <string>

#include "region.h"

class SQLRepository
{
public:
    SQLRepository();

    // 加载区域数据
    bool loadSQLcountries(const std::string& path);

    // 返回活跃区域
    std::vector<Region> regionsAt(int year) const;

private:
    std::vector<Region> m_regions;  // 所有区域信息的存储
};


