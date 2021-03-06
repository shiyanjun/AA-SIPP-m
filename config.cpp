#include "config.h"
using namespace tinyxml2;

Config::Config()
{
    N = -1;
    searchParams = 0;
}

Config::~Config()
{
    if (searchParams)
    {
        delete[] searchParams;
    }
}

bool Config::getConfig(const char* FileName)
{
    std::string value;
    float weight;
    float loglevel;
    std::stringstream stream;

    XMLDocument doc;
    if(doc.LoadFile(FileName) != XMLError::XML_SUCCESS)
    {
        std::cout << "Error openning input XML file."<<std::endl;
        return false;
    }

    XMLElement *root = doc.FirstChildElement(CNS_TAG_ROOT);
    if (!root)
    {
        std::cout << "No 'root' element found in XML file."<<std::endl;
        return false;
    }

    XMLElement *algorithm = root->FirstChildElement(CNS_TAG_ALGORITHM);
    if (!algorithm)
    {
        std::cout << "No 'algorithm' element found in XML file."<<std::endl;
        return false;
    }

    XMLElement *element;

    N = CN_PT_NUM;
    searchParams = new float[N];

    element = algorithm->FirstChildElement(CNS_TAG_ALLOW_AA);
    if (!element)
    {
        std::cout << "Error! No '"<<CNS_TAG_ALLOW_AA<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to 'true'."<<std::endl;
        searchParams[CN_PT_AA] = 1;
    }
    else
    {
        value = element->GetText();
        if(value == "true" || value == "1")
            searchParams[CN_PT_AA] = 1;
        else if(value == "false" || value == "0")
            searchParams[CN_PT_AA] = 0;
        else
        {
            std::cout << "Warning! Wrong '"<<CNS_TAG_ALLOW_AA<<"' value. It's compared to 'true'."<<std::endl;
            searchParams[CN_PT_AA] = 1;
        }
    }

    element = algorithm->FirstChildElement(CNS_TAG_METRICTYPE);
    if (!element)
    {
        if(searchParams[CN_PT_AA] == 1.0)
        {
            std::cout << "Error! No '"<<CNS_TAG_METRICTYPE<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to '"<< CNS_MT_EUCLID <<"'."<<std::endl;
            searchParams[CN_PT_MT]=CN_MT_EUCLID;
        }
        else
        {
            std::cout << "Error! No '"<<CNS_TAG_METRICTYPE<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to '"<< CNS_MT_MANHATTAN <<"'."<<std::endl;
            searchParams[CN_PT_MT]=CN_MT_MANHATTAN;
        }
    }
    else
    {
        value = element->GetText();
        if(value == CNS_MT_EUCLID)
            searchParams[CN_PT_MT] = CN_MT_EUCLID;
        else if(value == CNS_MT_DIAGONAL)
            searchParams[CN_PT_MT] = CN_MT_DIAGONAL;
        else if(value == CNS_MT_MANHATTAN)
            searchParams[CN_PT_MT] = CN_MT_MANHATTAN;
        else
        {
            if(searchParams[CN_PT_AA] == 1.0)
            {
                std::cout << "Warning! Wrong '"<<CNS_TAG_METRICTYPE<<"' value. It's compared to '"<< CNS_MT_EUCLID <<"'."<<std::endl;
                searchParams[CN_PT_MT] = CN_MT_EUCLID;
            }
            else
            {
                std::cout << "Warning! Wrong '"<<CNS_TAG_METRICTYPE<<"' value. It's compared to '"<< CNS_MT_MANHATTAN <<"'."<<std::endl;
                searchParams[CN_PT_MT] = CN_MT_MANHATTAN;
            }
        }
    }

    element = algorithm->FirstChildElement(CNS_TAG_CONSTRAINTSTYPE);
    if (!element)
    {
        std::cout << "Warning! No '"<<CNS_TAG_CONSTRAINTSTYPE<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to 'section'."<<std::endl;
        searchParams[CN_PT_CT] = CN_CT_SECTION;
    }
    else
    {
        value = element->GetText();
        if(value == CNS_CT_POINT)
            searchParams[CN_PT_CT] = CN_CT_POINT;
        else if(value == CNS_CT_VELOCITY)
            searchParams[CN_PT_CT] = CN_CT_VELOCITY;
        else if(value == CNS_CT_SECTION)
            searchParams[CN_PT_CT] = CN_CT_SECTION;
        else
        {
            std::cout << "Warning! Wrong '"<<CNS_TAG_CONSTRAINTSTYPE<<"' value. It's compared to 'section'."<<std::endl;
            searchParams[CN_PT_CT] = CN_CT_SECTION;
        }
    }

    element = algorithm->FirstChildElement(CNS_TAG_STARTSAFEINTERVAL);
    if (!element)
    {
        std::cout << "Warning! No '"<<CNS_TAG_STARTSAFEINTERVAL<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to 0."<<std::endl;
        searchParams[CN_PT_SSF] = 0;
    }
    else
    {
        value = element->GetText();
        stream<<value;
        stream>>searchParams[CN_PT_SSF];
        stream.clear();
        stream.str("");
    }

    element = algorithm->FirstChildElement(CNS_TAG_PRIORITIZATION);
    if (!element)
    {
        std::cout << "Warning! No '"<<CNS_TAG_PRIORITIZATION<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to 'fifo'."<<std::endl;
        searchParams[CN_PT_IP] = CN_IP_FIFO;
    }
    else
    {
        value = element->GetText();
        if(value == CNS_IP_FIFO)
            searchParams[CN_PT_IP] = CN_IP_FIFO;
        else if(value == CNS_IP_LONGESTF)
            searchParams[CN_PT_IP] = CN_IP_LONGESTF;
        else if(value == CNS_IP_SHORTESTF)
            searchParams[CN_PT_IP] = CN_IP_SHORTESTF;
        else if(value == CNS_IP_RANDOM)
            searchParams[CN_PT_IP] = CN_IP_RANDOM;
        else
        {
            std::cout << "Warning! Wrong '"<<CNS_TAG_PRIORITIZATION<<"' value. It's compared to 'fifo'."<<std::endl;
            searchParams[CN_PT_IP] = CN_IP_FIFO;
        }
    }

    element = algorithm->FirstChildElement(CNS_TAG_TIMELIMIT);
    if (!element)
    {
        std::cout << "Warning! No '"<<CNS_TAG_TIMELIMIT<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to -1(no limit)."<<std::endl;
        searchParams[CN_PT_TL] = -1;
    }
    else
    {
        value = element->GetText();
        stream<<value;
        stream>>searchParams[CN_PT_TL];
        stream.clear();
        stream.str("");
    }

    element = algorithm->FirstChildElement(CNS_TAG_RESCHEDULING);
    if (!element)
    {
        std::cout << "Warning! No '"<<CNS_TAG_RESCHEDULING<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to 'no'."<<std::endl;
        searchParams[CN_PT_RE] = CN_RE_NO;
    }
    else
    {
        value = element->GetText();
        if(value == CNS_RE_NO)
            searchParams[CN_PT_RE] = CN_RE_NO;
        else if(value == CNS_RE_RULED)
            searchParams[CN_PT_RE] = CN_RE_RULED;
        else if(value == CNS_RE_RANDOM)
            searchParams[CN_PT_RE] = CN_RE_RANDOM;
        else
        {
            std::cout << "Warning! Wrong '"<<CNS_TAG_RESCHEDULING<<"' value. It's compared to 'no'."<<std::endl;
            searchParams[CN_PT_RE] = CN_RE_NO;
        }
    }

    element = algorithm->FirstChildElement(CNS_TAG_WEIGHT);
    if (!element)
    {
        element = algorithm->FirstChildElement(CNS_TAG_HWEIGHT);
        if (!element)
        {
            std::cout << "Warning! No '"<<CNS_TAG_WEIGHT<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to "<< 1 <<"."<<std::endl;
            weight = 1;
        }
        else
        {
            value = element->GetText();
            stream<<value;
            stream>>weight;
            stream.clear();
            stream.str("");
        }
    }
    else
    {
        value = element->GetText();
        stream<<value;
        stream>>weight;
        stream.clear();
        stream.str("");
    }

    if (weight == 0)
    {
        std::cout << "Warning! Wrong '"<<CNS_TAG_WEIGHT<<"' value. It's compared to " << 1 <<"."<<std::endl;
        weight = 1;
    }
    searchParams[CN_PT_WEIGHT] = weight;

    element = algorithm->FirstChildElement(CNS_TAG_TURNINGWEIGHT);
    if (!element)
    {
        std::cout << "Warning! No '"<<CNS_TAG_TURNINGWEIGHT<<"' element found inside '"<<CNS_TAG_ALGORITHM<<"' section. It's compared to 0."<<std::endl;
        searchParams[CN_PT_TW] = 0;
    }
    else
    {
        value = element->GetText();
        stream<<value;
        stream>>searchParams[CN_PT_TW];
        stream.clear();
        stream.str("");
    }

    XMLElement *options = root->FirstChildElement(CNS_TAG_OPTIONS);
    if(!options)
    {
        std::cout << "No '"<<CNS_TAG_OPTIONS<<"' element found in XML file."<<std::endl;
        return false;
    }

    element = options->FirstChildElement(CNS_TAG_LOGLVL);
    if(!element)
    {
        std::cout << "No '"<<CNS_TAG_LOGLVL<<"' element found in XML file."<<std::endl;
        return false;
    }

    value = element->GetText();
    stream<<value;
    stream>>loglevel;
    stream.clear();
    stream.str("");

    searchParams[CN_PT_LOGLVL] = loglevel;

    return true;
}
