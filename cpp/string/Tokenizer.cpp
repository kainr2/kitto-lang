
#include <iostream>
#include <vector>
#include <algorithm>



class Tokenizer
{
public:
    std::vector<std::string> CharDelimiter(
        const std::string& source,
        const std::string& delim=" ");
};



using namespace std;

vector<string> Tokenizer::CharDelimiter(
    const string& source,
    const string& delim)
{
    vector<string> output;
    size_t head = 0;
    size_t curr = 0;

    // function ptr
    // *http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible
    // * http://www.cplusplus.com/reference/string/string/find/
    // * http://www.cplusplus.com/reference/string/string/find_first_of/
    size_t (string::*fp)(const string&, size_t) const;
    fp = &string::find_first_of;

    do 
    {
        // Find the string
        //curr = source.find_first_of(delim, head);
        curr = (source.*fp)(delim, head);
        
        // If there is no more match...
        if (curr==string::npos)
        {
            // If there is something to copy
            string last = source.substr(head);
            if (!last.empty())
            {
                output.push_back(last);
            }
            break;
        }

        // Otherwise, there is something to copy
        if (curr!=head)
        {
            output.push_back(source.substr(head,curr-head));
        }

        // Move the position (beyond the delim)
        head = curr+1;
    } while (true);

    // Return the result
    return output;
}
////////////////////////////////////////////////////////////////////////////////


void CharDelimiterTester(
    const std::string& source,
    const std::string& delim)
{
    Tokenizer test;
    vector<string> output = test.CharDelimiter(source, delim);

    cout << "original delim[" << delim << "] and string[" << source << "]\n";
    for (int i=0; i<output.size(); i++)
    {
        cout << "token = " << output[i] << endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
int main()
{
    std::string source1 = "hel-lo";
    std::string delim  = "-";
    
    // test basic, middle split
    CharDelimiterTester("he-l-lo", "-");
    CharDelimiterTester("a-bc/def/ghi", "-/");
    CharDelimiterTester("hel-o", "-");

    // test multiple delim
    CharDelimiterTester("hel--abc/--lo", "-/");

    // test delim prefix and postfix
    CharDelimiterTester("/h/a/b/c/--lo-/", "-/");
    CharDelimiterTester("---/buy-bye-love", "-/");
    CharDelimiterTester("oh-my-god/////", "-/");

    // test single char
    CharDelimiterTester("a", "/**");
    CharDelimiterTester("b", "");
    CharDelimiterTester("", "*");
    CharDelimiterTester("", "");

    return 0;
}
