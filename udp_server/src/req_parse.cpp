#include "req_parse.h"

#include <stdio.h>

int Parse::ParseReq(char *req, int reqLen)
{
    if (req == nullptr) {
        std::cout << "输入参数非法" << std::endl;
        return -1;
    }
    sscanf(req, "%s %s %s", m_methond, m_uri, m_params);

    std::cout << "methond: " << m_methond << std::endl;
    std::cout << "uri: " << m_uri << std::endl;
    std::cout << "params: " << m_params << std::endl;

    return 0;
}

std::string Parse::GetMethond()
{
    return std::string(m_methond);
}

std::string Parse::GetUri()
{
    return std::string(m_uri);
}

std::string Parse::GetParam()
{
    return std::string(m_params);
}