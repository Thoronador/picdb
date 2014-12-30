#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype> //for std::isprint()
#include "../../common/escaping.hpp"

std::string make_printable(std::string str)
{
  std::string::size_type i = 0;
  while (i<str.size())
  {
    if (!std::isprint(str[i]))
    {
      std::stringstream strstr;
      strstr << "[" << (int) str[i] << "]";
      str.replace(i, 1, strstr.str());
    }
    ++i;
  } //while
  return str;
}

int main()
{
  std::string str1;
  std::vector<std::string> test_strings;
  test_strings.push_back("");
  test_strings.push_back("abcdefghijklmnopqrstuvwxyz0123456789");
  test_strings.push_back("this is a test");
  // one NUL character
  str1 = "a";
  str1[0] = '\0';
  test_strings.push_back(str1);
  //one backslash
  test_strings.push_back("\\");
  //five backslashes
  test_strings.push_back("\\\\\\\\\\\\");
  //five NUL characters
  str1 = "12345";
  str1[0] = '\0'; str1[1] = '\0'; str1[2] = '\0'; str1[3] = '\0'; str1[4] = '\0';
  test_strings.push_back(str1);
  //five NUL characters and four backshashes between them, i.e. nine characters, altering between NUL and backslash
  str1 = "1\\2\\3\\4\\5";
  str1[0] = '\0'; str1[2] = '\0'; str1[4] = '\0'; str1[6] = '\0'; str1[8] = '\0';
  test_strings.push_back(str1);

  //iterate over all strings and check, if they are (un-)escaped properly
  unsigned int i;
  for (i=0; i<test_strings.size(); ++i)
  {
    const std::string esc = escape(test_strings[i]);
    const std::string unesc = unescape(esc);
    if (i!=0)
      std::cout << std::endl;
    std::cout << "original:    " << make_printable(test_strings[i]) << std::endl
              << "escaped:     " << make_printable(esc) << std::endl
              << "unescaped:   " << make_printable(unesc) << std::endl;
    const bool match = (test_strings[i]==unesc);
    std::cout << "orig==unesc: " << match << std::endl;
    //check, if unescaped escaped string matches original
    if (!match)
      return 1+i;
  }//for
  //success
  return 0;
}
