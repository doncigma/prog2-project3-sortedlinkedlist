#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "unsortedlinkedlist.hpp"
#include "sortedlinkedlist.hpp"
#include "linkedlisttest.hpp"

using namespace std;

bool TestAppend(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestPrepend(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestInsertAt(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestInsert(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestRemove(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestGet(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestSize(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestClear(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestEmpty(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestFind(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestFindIndex(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestForeach(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestPrint(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestOrder(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestSort(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);

vector<TestFunctionEntry> linkedListTestCommands = {
    {"append", "append [u] <value>", TestAppend},
    {"prepend", "prepend [u] <value>", TestPrepend},
    {"insertat", "insertat [u] <value> <position>", TestInsertAt},
    {"insert", "insert [s | p | e] <value>>", TestInsert},
    {"order", "order [p | e] <value>>", TestOrder},
    {"sort", "sort [e] <value>>", TestSort},
    {"removeat", "removeat [s | u | p | e] <value> <position>", TestRemove},
    {"size", "size [s | u | p | e]", TestSize},
    {"empty", "empty [s | u | p | e]", TestEmpty},
    {"clear", "clear [s | u | p | e]", TestClear},
    {"get", "get [s | u | p | e] <position>", TestGet},
    {"[]", "[] [s | u | p | e] <position>", TestGet},
    {"find", "find [s | u | p | e] <value>", TestFind},
    {"findindex", "findindex [s | u | p | e] <value>", TestFindIndex},
    {"foreach", "foreach [s | u | p | e]", TestForeach},
    {"print", "print [s | u | p | e]", TestPrint},
};

class MyString
{
public:
    MyString(const string &value) : _str(value) {}
    MyString(const char *value) : _str(value) {}
    MyString(const MyString &value) : _str(value._str) {}
    bool operator<(const MyString &right) const
    {
        return Compare(right) < 0;
    }

    bool operator==(const MyString &right) const
    {
        return Compare(right) == 0;
    }

    bool operator!=(const MyString &right) const
    {
        return Compare(right) != 0;
    }

    bool operator>(const MyString &right) const
    {
        return Compare(right) > 0;
    }
    
    // << operator overloading
    friend ostream &operator<<(ostream &os, const MyString &myString)
    {
        os << myString.GetString();
        return os;
    }
    
    // >> operator overloading
    friend istream &operator>>(istream &is, MyString &myString)
    {
        is >> myString._str;
        return is;
    }
    string GetString() const { return _str; }

    int length() const { return _str.length(); }

    const string &operator=(const string &value)
    {
        _str = value;
        return _str;
    }

    operator string()
    {
        return _str;
    }

protected:
    virtual int Compare(const MyString &right) const
    {
        int leftInt = stoi(GetString());
        int rightInt = stoi(right.GetString());
        return leftInt < rightInt ? -1 : (leftInt == rightInt ? 0 : 1);
    }

private:
    string _str;
};

int IntCompare(const MyString &leftString, const MyString &rightString)
{
    int left = stoi(leftString.GetString());
    int right = stoi(rightString.GetString());
    return left < right ? -1 : (left == right ? 0 : 1);
}

int StringCompare1(const MyString &left, const MyString &right)
{
    string leftString = left.GetString();
    string rightString = right.GetString();

    return leftString < rightString ? -1 : (leftString == rightString ? 0 : 1);
}

int StringCompare2(const MyString &left, const MyString &right)
{
    int leftLen = left.length();
    int rightLen = right.length();

    if (leftLen != rightLen)
    {
        return leftLen < rightLen ? -1 : 1;
    }

    return StringCompare1(left, right);
}

UnsortedLinkedList<MyString> myNameList;
SortedLinkedListProperty<MyString> mypNameList(IntCompare);
SortedLinkedList<MyString> mysNameList;
SortedLinkedListProperty<MyString> myeNameList(StringCompare1);

enum ListType
{
    ListInvalid = 0,
    ListUnsorted,
    ListSortedVirtual,
    ListSortedProperty,
    ListSortedPropertyExtra
};

string uParam = string("u");
string sParam = string("s");
string pParam = string("p");
string eParam = string("e");

ListType ParamToListType(const string &s)
{
    if (s == uParam)
    {
        return ListUnsorted;
    }
    else if (s == sParam)
    {
        return ListSortedVirtual;
    }
    else if (s == pParam)
    {
        return ListSortedProperty;
    }
    else if (s == eParam)
    {
        return ListSortedPropertyExtra;
    }
    else
    {
        return ListInvalid;
    }
}

LinkedListBase<MyString> &ParamToLinkedList(const string &s)
{
    switch (ParamToListType(s))
    {
    case ListUnsorted:
        return myNameList;
    case ListSortedVirtual:
        return mysNameList;
    case ListSortedProperty:
        return mypNameList;
    case ListSortedPropertyExtra:
        return myeNameList;
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }
}

SortedLinkedListBase<MyString> &ParamToSortedLinkedList(const string &s)
{
    switch (ParamToListType(s))
    {
    case ListUnsorted:
        throw invalid_argument("u is not supported on sorted linked list");
    case ListSortedVirtual:
        return mysNameList;
    case ListSortedProperty:
        return mypNameList;
    case ListSortedPropertyExtra:
        return myeNameList;
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }
}

bool TestAppend(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw invalid_argument("append requires 2 parameter");
    }

    switch (ParamToListType(params[0]))
    {
    case ListUnsorted:
        myNameList.Append(params[1]);
        break;
    case ListSortedVirtual:
        throw invalid_argument("s not supported on append");
        break;
    case ListSortedProperty:
        throw invalid_argument("p not supported on append");
        break;
    case ListSortedPropertyExtra:
        throw invalid_argument("e not supported on append");
        break;
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }

    output = "";
    return true;
}

bool TestPrepend(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw invalid_argument("prepend requires 2 parameter");
    }

    switch (ParamToListType(params[0]))
    {
    case ListUnsorted:
        myNameList.Prepend(params[1]);
        break;
    case ListSortedVirtual:
        throw invalid_argument("s not supported on prepend");
        break;
    case ListSortedProperty:
        throw invalid_argument("p not supported on prepend");
        break;
    case ListSortedPropertyExtra:
        throw invalid_argument("e not supported on prepend");
        break;
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }

    output = "";
    return true;
}

bool TestInsertAt(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 3)
    {
        throw invalid_argument("insertat requires 3 parameter");
    }

    switch (ParamToListType(params[0]))
    {
    case ListUnsorted:
        myNameList.InsertAt(params[1], stoi(params[2]));
        break;
    case ListSortedVirtual:
        throw invalid_argument("s not supported on insertat");
        break;
    case ListSortedProperty:
        throw invalid_argument("p not supported on insertat");
        break;
    case ListSortedPropertyExtra:
        throw invalid_argument("e not supported on insertat");
        break;
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }

    output = "";
    return true;
}

bool TestInsert(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw invalid_argument("insert requires 2 parameter");
    }

    switch (ParamToListType(params[0]))
    {
    case ListUnsorted:
        throw invalid_argument("u not supported on insert");
        break;
    case ListSortedProperty:
    case ListSortedVirtual:
    case ListSortedPropertyExtra:
        ParamToSortedLinkedList(params[0]).Insert(params[1]);
        break;
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }

    output = "";
    return true;
}

bool TestRemove(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw invalid_argument("removeat requires 2 parameter");
    }

    ParamToLinkedList(params[0]).RemoveAt(stoi(params[1]));

    output = "";
    return true;
}

bool TestGet(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw invalid_argument("get requires 2 parameter");
    }

    output = ParamToLinkedList(params[0]).Get(stoi(params[1]));

    return true;
}

bool TestSize(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw invalid_argument("size requires 1 parameter");
    }

    output = to_string(ParamToLinkedList(params[0]).Size());

    return true;
}

bool TestClear(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw std::invalid_argument("size requires 1 parameter");
    }

    ParamToLinkedList(params[0]).Clear();
    output = "";

    return true;
}

bool TestEmpty(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw std::invalid_argument("empty requires 1 parameter");
    }

    output = to_string(ParamToLinkedList(params[0]).Empty());

    return true;
}

bool TestFind(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw std::invalid_argument("find requires 2 parameters");
    }

    switch (ParamToListType(params[0]))
    {
    case ListUnsorted:
    case ListSortedVirtual:
    case ListSortedProperty:
    {
        int valueToFind = stoi(params[1]);
        auto predicate = [&](const string &value)
        { return stoi(value) == valueToFind; };

        output = ParamToLinkedList(params[0]).Find(predicate);
        break;
    }
    case ListSortedPropertyExtra:
    {
        string valueToFind = params[1];
        auto predicate = [&](const string &value)
        { return value == valueToFind; };
        output = ParamToLinkedList(params[0]).Find(predicate);
        break;
    }
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }

    return true;
}

bool TestFindIndex(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw std::invalid_argument("findindex requires 2 parameters");
    }

    switch (ParamToListType(params[0]))
    {
    case ListUnsorted:
    case ListSortedVirtual:
    case ListSortedProperty:
    {
        int valueToFind = stoi(params[1]);
        auto predicate = [&](const string &value)
        { return stoi(value) == valueToFind; };

        output = to_string(ParamToLinkedList(params[0]).FindIndex(predicate));
        break;
    }
    case ListSortedPropertyExtra:
    {
        string valueToFind = params[1];
        auto predicate = [&](const string &value)
        { return value == valueToFind; };
        output = to_string(ParamToLinkedList(params[0]).FindIndex(predicate));
        break;
    }
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }

    return true;
}

bool TestForeach(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw std::invalid_argument("foreach requires 1 parameter");
    }

    output = "";
    ParamToLinkedList(params[0]).ForEach([&output](string value)
                                         { output.append(value + ","); });

    return true;
}

bool TestPrint(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw std::invalid_argument("print requires 1 parameter");
    }

    output = "";
    ParamToLinkedList(params[0]).ForEach([&output](string value)
                                         { output.append(value + ","); });

    return true;
}

bool TestOrder(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw invalid_argument("order requires 2 parameter");
    }

    switch (ParamToListType(params[0]))
    {
    case ListUnsorted:
        throw invalid_argument("u not supported on order");
        break;
    case ListSortedVirtual:
        throw invalid_argument("s not supported on order");
        break;
    case ListSortedProperty:
        mypNameList.SetOrder(stoi(params[1]));
        break;
    case ListSortedPropertyExtra:
        myeNameList.SetOrder(stoi(params[1]));
        break;
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }

    output = "";
    return true;
}

bool TestSort(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw invalid_argument("sort requires 2 parameter");
    }

    switch (ParamToListType(params[0]))
    {
    case ListUnsorted:
        throw invalid_argument("u not supported on sort");
        break;
    case ListSortedVirtual:
        throw invalid_argument("s not supported on sort");
        break;
    case ListSortedProperty:
        throw invalid_argument("p not supported on sort");
        break;
    case ListSortedPropertyExtra:
        switch (stoi(params[1]))
        {
        case 1:
            myeNameList.SetCompareFunction(StringCompare1);
            break;
        case 2:
            myeNameList.SetCompareFunction(StringCompare2);
            break;
        default:
            throw invalid_argument("invalid sort type - expected 1 for regular sort or 2 for sort on length");
        }
        break;
    default:
        throw invalid_argument("invalid list type - expected e, u, s, or p");
    }

    output = "";
    return true;
}
