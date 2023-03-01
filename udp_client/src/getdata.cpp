#include "getdata.h"


char *UdpData::GetSpecifyPic()
{
    char method[DATA_SIZE] = "Get";
    char uri[DATA_SIZE] = "server/getpic";
    char index[DATA_SIZE] = "{index:0}";
    char *req = (char *)malloc(strlen(method) + strlen(uri) + strlen(index) + 1);
    int ret = sprintf(req, "%s %s %s",method, uri, index);
    if (ret == -1) {
        std::cout << " sprintf error !!! " << std::endl;
        free(req);
        req == NULL;
        return nullptr;
    }

    return req;
}