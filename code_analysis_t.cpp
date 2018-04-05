/*
  @file code_analysis_t.cpp

  Test program for analysis requests
*/

#include "code_analysis.hpp"

#include <string>
#include <cassert>

int main() {

    // all parts of the request are empty
    {
        analysis_request request;
        request.given_filename  = "";
        request.entry_filename  = "";
        request.given_url       = "";
        request.option_filename = "";
        request.option_url      = "";
        request.option_language = "";

        auto filename = analysis_filename(request);
        assert(filename == "");
        assert(analysis_url(request) == "");
        assert(analysis_language(request, filename) == "");
        assert(code_analysis(request) == false);
    }

    // only option_filename filled in
    {
        analysis_request request;
        request.given_filename  = "";
        request.entry_filename  = "";
        request.given_url       = "";
        request.option_filename = "option_filename";
        request.option_url      = "";
        request.option_language = "";

        auto filename = analysis_filename(request);
        assert(filename == "option_filename");
        assert(analysis_url(request) == "");
        assert(analysis_language(request, filename) == "");
        assert(code_analysis(request) == false);
    }

    // given_filename (as source-code file) and entry_filename filled in
    {
        analysis_request request;
        request.given_filename  = "given_filename.zip";
        request.entry_filename  = "entry_filename";
        request.given_url       = "";
        request.option_filename = "";
        request.option_url      = "";
        request.option_language = "";

        auto filename = analysis_filename(request);
        assert(filename == "entry_filename");
        assert(analysis_url(request) == "");
        assert(analysis_language(request, filename) == "");
        assert(code_analysis(request) == false);
    }

    // given_filename (as regular file) and entry_filename filled in
    {
        analysis_request request;
        request.given_filename  = "given_filename.cpp";
        request.entry_filename  = "data";
        request.given_url       = "";
        request.option_filename = "";
        request.option_url      = "";
        request.option_language = "";

        auto filename = analysis_filename(request);
        assert(filename == "given_filename.cpp");
        assert(analysis_url(request) == "");
        assert(analysis_language(request, filename) == "C++");
        assert(code_analysis(request) == false);
    }

    // given_url filled in
    {
        analysis_request request;
        request.given_filename  = "";
        request.entry_filename  = "";
        request.given_url       = "given_url";
        request.option_filename = "";
        request.option_url      = "";
        request.option_language = "";

        auto filename = analysis_filename(request);
        assert(filename == "");
        assert(analysis_url(request) == "given_url");
        assert(analysis_language(request, filename) == "");
        assert(code_analysis(request) == false);
    }

    // given_url and option_url filled in
    {
        analysis_request request;
        request.given_filename  = "";
        request.entry_filename  = "";
        request.given_url       = "given_url";
        request.option_filename = "";
        request.option_url      = "option_url";
        request.option_language = "";

        auto filename = analysis_filename(request);
        assert(filename == "");
        assert(analysis_url(request) == "option_url");
        assert(analysis_language(request, filename) == "");
        assert(code_analysis(request) == false);
    }

    // option_language filled in
    {
        analysis_request request;
        request.given_filename  = "";
        request.entry_filename  = "";
        request.given_url       = "";
        request.option_filename = "";
        request.option_url      = "";
        request.option_language = "option_language";

        auto filename = analysis_filename(request);
        assert(filename == "");
        assert(analysis_url(request) == "");
        assert(analysis_language(request, filename) == "option_language");
        assert(code_analysis(request) == false);
    }

    // test for stdin for regular file and option_filename specified
    {
        analysis_request request;
        request.given_filename  = "-";
        request.entry_filename  = "data";
        request.given_url       = "";
        request.option_filename = "option_filename";
        request.option_url      = "";
        request.option_language = "";

        auto filename = analysis_filename(request);
        assert(filename == "option_filename");
        assert(analysis_url(request) == "");
        assert(analysis_language(request, filename) == "");
        assert(code_analysis(request) == false);
    }

    return 0;
}
