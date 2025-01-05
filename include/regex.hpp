
#pragma once

#include <regex>
#include <string>
#include <string_view>
#include <vector>


namespace jademl {


class Regex
    :   public std::string
{
public:
    enum FlagType {
        Multiline,
        Singleline
    } flag;

    Regex( const char* str );
    Regex( const char* str, const FlagType flag_type );

    std::regex_constants::syntax_option_type type( const FlagType flag_type = Singleline ) const;

    Regex& operator=( const char* str );
    Regex& operator+( const Regex& other );
    Regex& operator+( const char* other );

    Regex& operator<<( const char* other );
    Regex& operator<<( const std::string other );
    Regex& operator<<( const Regex& other );

    std::string_view match( const char* str ) const;
    std::vector< std::string_view >& matchall( const char* str ) const;

    typedef typename std::vector< std::string_view >::const_iterator match_iterator;
};


#define make_regex(var_name) inline jademl::Regex var_name = 
#define make_sregex(var_name) inline jademl::Regex var_name = jademl::Regex("") << 

#define Matchall(str, var_name, regexp) std::vector< std::string_view >& var_name \
                                    = regexp.matchall( str );
#define Match(str, var_name, regexp) std::string_view var_name = regexp.match( str );



class RegexAdvanced
    :   public Regex
{
public:
    enum FlagMode {
        Preamble,
        SetIn
    } mode = SetIn;

    RegexAdvanced( const char* str );
    RegexAdvanced( const char* str, const FlagMode flag_mode = SetIn, const FlagType flag_type = Singleline );
    RegexAdvanced( const char* str, const FlagType = Singleline );

    RegexAdvanced& operator=( const char* str );
    RegexAdvanced& operator+( const Regex& other );
    RegexAdvanced& operator+( const RegexAdvanced& other );
    RegexAdvanced& operator+( const char* other );

    RegexAdvanced& operator<<( const char* other );
    RegexAdvanced& operator<<( const std::string other );
    RegexAdvanced& operator<<( const Regex& other );
    RegexAdvanced& operator<<( const RegexAdvanced& other );
    
    std::string_view match( const char* str, const char* in_between ) const;
    std::vector< std::string_view >& matchall( const char* str, const char* in_between ) const;
};


#define make_regexadv(var_name) inline jademl::RegexAdvanced var_name = 
#define make_sregexadv(var_name) inline jademl::RegexAdvanced var_name = jademl::RegexAdvanced("") << 

#define MatchallAdv(str, var_name, regexp, setin_var) std::vector< std::string_view >& var_name \
                                    = regexp.matchall( str, setin_var );
#define MatchAdv(str, var_name, regexp, setin_var) std::string_view var_name = regexp.match( str, setin_var );



namespace regex {

make_regex(no_indent)                   "(? ! (^(( )+)*))"; // Matches everything that is thus not indented
make_sregex(multiline_text)             "((? = (^[\d\w\s]$)+ )( ? ! " + no_indent + "\|(\s+)))(\n\n+)";


namespace CSS {
make_regex(html_id)                     "#(\w)+";
make_regex(css_cls)                     "\w+";
make_regex(css_cls_dotted)              ".(\w)+";
make_sregex(css_clses_dotted)           "(" + css_cls_dotted + ")+";
make_regex(css_cls_dotted_indented)     regex::no_indent + css_cls_dotted;
make_sregex(css_cls_multiline)          "(\n)*(" + css_cls_dotted_indented + ")[\n|\s]+";
make_regex(html_id_multiline_css)       regex::no_indent + html_id + css_cls_multiline;
} // namespace CSS


namespace JADE {

make_regex(bracket_content)              "(\([\w,\s]+\))?";
make_sregex(html_id_with_css)            "(" + CSS::css_cls_multiline + "|" + CSS::html_id_multiline_css + ")?";
make_regex(html_id_css)                  CSS::html_id + CSS::css_clses_dotted;
make_sregex(html_id_no_css)              "(( ? ! #)?\w+)(( ? ! " + CSS::css_clses_dotted + ")?)";
make_regex(jade_tag_name)                "\w+\s?";
make_sregex(jade_tag)                    "\w+\s?((" + CSS::html_id + ")?(" \
                                            + CSS::css_clses_dotted + ")?" + bracket_content + "?)\s+";
make_sregex(jade_tag_with_content)       jade_tag + "[\w\s]+( ? ! " + jade_tag + ")+";
} // namespace JADE


namespace JINJA {

make_regex(brace_delimiters)             "\{\%(\s*)[\w\d\s](\s*)\%\}";
make_regex(brace_content)                "( ? ! [\s\w\d]?( ? = \{\%(\s+) ) )" \
                                            "[\w\s\d\|\:\=\!\{\}\+\-\*\/]+" \
                                            "( ? ! [\s\w\d]?( ? = (\s+)\%\} ) )";
make_regex(brace_content_var)                "( ? ! [\s\w\d]?( ? = \{\%(\s+) ) )" \
                                            "[\w\s\d\|\:\=\!\{\}]+" \
                                            "( ? ! [\s\w\d]?( ? = (\s+)\%\} ) )";
make_regex(brace_content_func)           "( ? ! [\s\w\d]?( ? = \{\+(\s+) ) )" \
                                            "[\w\s\d\|\:\=\!\{\}]+" \
                                            "( ? ! [\s\w\d]?( ? = (\s+)\+\} ) )";
} // namespace JINJA

} // namespace regex
} // namespace jademl
