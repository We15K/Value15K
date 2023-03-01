#pragma once

#include <iostream>

class Parse {
public:
    int ParseReq(char *req, int reqLen);

    std::string GetMethond();
    std::string GetUri();
    std::string GetParam();
private:
    char m_methond[8];
    char m_uri[128];
    char m_params[256];
};