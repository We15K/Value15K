#pragma once

#include <iostream>

class ReqMsg {
public:
    // int GetHandlerIndex(std::string method, std::string uri, std::string param);

private:
    int reqIndex;
    std::string m_head;
    std::string m_body;
    std::string m_param;
};