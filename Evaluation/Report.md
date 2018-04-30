
# TylerBis

## Files
% ls

    Makefile
    README.md
    analysis_request.hpp
    code_analysis.cpp
    code_analysis.hpp
    code_analysis_t.cpp
    get_language_from_filename.cpp
    get_language_from_filename.hpp
    get_language_from_filename_t.cpp

## Changed files
% git diff baa2b2fa58cb52ada94597a9b2e5d687f6a3261f --name-only

    code_analysis.cpp
    code_analysis_t.cpp

## Build
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

## Test
% make test

    ./code_analysis_t
    Extension not supported
    Extension not supported
    Extension not supported
    Extension not supported
    Extension not supported
    Using stdin requires a declared language

## Test Oracle: code\_analysis\_t.cpp
% cp ~collard/CodeAnalysisMetadataEvaluation/code_analysis_t.cpp .  
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

% make test

    ./code_analysis_t
    Extension not supported
    ./code_analysis_t: code_analysis_t.cpp:62: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:79: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:96: main Check `code_analysis(request) == true' failed.
    Extension not supported
    Using stdin requires a declared language
    ./code_analysis_t: code_analysis_t.cpp:130: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:147: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:164: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:181: main Check `code_analysis(request) == true' failed.

## Test Oracle: code\_analysis.cpp
% cp ~collard/CodeAnalysisMetadataEvaluation/code_analysis.cpp  .  
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

% make test

    ./code_analysis_t
    Extension not supported
    Assertion failed: (code_analysis(request) == false), function main, file code_analysis_t.cpp, line 45.
    make: *** [test] Abort trap: 6

## Code
% srcml code_analysis.cpp code_analysis_t.cpp -o project.xml
% srcml --xpath="//src:function[src:name='code_analysis']" project.xml | srcml

     1 bool code_analysis(const analysis_request& request) {
     2 
     3     auto filename = analysis_filename(request);
     4 
     5     auto url = analysis_url(request);
     6 
     7     auto language = analysis_language(request, filename);
     8 
     9     // code analysis processing that is not yet implemented
    10 
    11     // error handling
    12     if (language == "") {
    13       std::cerr << "Extension not supported\n";
    14       if (request.given_filename == "-")
    15         std::cerr << "Using stdin requires a declared language\n";
    16     return false;
    17     }
    18 
    19     return false;
    20 }

% srcml --xpath="//src:function[src:name='analysis_filename']" project.xml | srcml

     1 std::string analysis_filename(const analysis_request& request) {
     2     if (request.given_filename == "-" && request.entry_filename == "data" && request.option_filename == "")
     3       return "";
     4     if (request.option_filename != "")
     5       return request.option_filename;
     6     std::string ext = get_language_from_filename(request.given_filename);
     7     if (ext != "")
     8       return request.given_filename;
     9     return request.entry_filename;
    10 }

% srcml --xpath="//src:function[src:name='analysis_url']" project.xml | srcml

     1 std::string analysis_url(const analysis_request& request) {
     2     if (request.option_url != "")
     3       return request.option_url;
     4     return request.given_url;
     5 }

% srcml --xpath="//src:function[src:name='analysis_language']" project.xml | srcml

     1 std::string analysis_language(const analysis_request& request, const std::string& filename) {
     2     if (request.option_language != "")
     3       return request.option_language;
     4     return get_language_from_filename(filename);
     5 }

## Test Cases 
% srcml code_analysis_t.cpp --xpath="//src:function[src:name='main']/src:block" | srcml

    {
    
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
            request.option_filename = "option_filename.cpp";
            request.option_url      = "";
            request.option_language = "";
    
            auto filename = analysis_filename(request);
            assert(filename == "option_filename.cpp");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
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
            request.option_language = "C++";
    
            auto filename = analysis_filename(request);
            assert(filename == "");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
        }
    
        // test for stdin for regular file and option_filename specified
        {
            analysis_request request;
            request.given_filename  = "-";
            request.entry_filename  = "data";
            request.given_url       = "";
            request.option_filename = "option_filename.cpp";
            request.option_url      = "";
            request.option_language = "";
    
            auto filename = analysis_filename(request);
            assert(filename == "option_filename.cpp");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
        }
    
        // test for stdin for archive file
        {
            analysis_request request;
            request.given_filename  = "-";
            request.entry_filename  = "entry_filename.zip";
            request.given_url       = "";
            request.option_filename = "";
            request.option_url      = "";
            request.option_language = "";
    
            auto filename = analysis_filename(request);
            assert(filename == "entry_filename.zip");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "");
            assert(code_analysis(request) == false);
        }
    
        // test for stdin with entry_filename == "data" and option_filename == ""
        {
            analysis_request request;
            request.given_filename  = "-";
            request.entry_filename  = "data";
            request.given_url       = "";
            request.option_filename = "";
            request.option_url      = "";
            request.option_language = "C++";
    
            auto filename = analysis_filename(request);
            assert(filename == "");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
        }
    
        return 0;
    }

## Literal Values used in Testing
% srcml code\_analysis_t.cpp --xpath="//src:literal" | srcml | tr '\0' '\n' |grep '"' | sort -u | tr '\n' ','

    "-" "C++" "data" "entry_filename" "entry_filename.zip" 
    "given_filename.cpp" "given_filename.zip" "given_url" 
    "option_filename.cpp" "option_url" 

## Error Messages
% srcml code_analysis.cpp --xpath="//src:function[src:name='code_analysis']//src:expr[contains(., 'cerr')]//src:literal" | srcml | tr '\0'  '\n'

    "Extension not supported\n"
    "Using stdin requires a declared language\n"

## Commits

### Commit 2ef6f9
% git checkout -q 2ef6f9  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

% git show

    commit 2ef6f9f8951cf1d91543f0278f08189b390018be
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 15:08:46 2018 -0400
    
        option_filename test
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 820446d..04e96c6 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -28,5 +28,21 @@ int main() {
             assert(code_analysis(request) == false);
         }
     
    +    {
    +        analysis_request request;
    +        request.given_filename  = "";
    +        request.entry_filename  = "";
    +        request.given_url       = "";
    +        request.option_filename = "option_filename";
    +        request.option_url      = "";
    +        request.option_language = "";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "option_filename");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +    }
    +
         return 0;
     }


### Commit f99727
% git checkout -q f99727  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit f9972708b6c9f6107172003cd5f4e484bda3b1a4
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 15:52:05 2018 -0400
    
        option_filename fix
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index b874829..84f4894 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -33,7 +33,7 @@ bool code_analysis(const analysis_request& request) {
      */
     std::string analysis_filename(const analysis_request& request) {
     
    -    return "";
    +    return request.option_filename;
     }
     
     /** URL extracted from the request


### Commit 2c79d3
% git checkout -q 2c79d3  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 2c79d3f3d31a7446f8e2e00a259f9a4673274227
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:02:10 2018 -0400
    
        test for source archive
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 04e96c6..dd9e0d0 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -44,5 +44,21 @@ int main() {
             assert(code_analysis(request) == false);
         }
     
    +    {
    +        analysis_request request;
    +        request.given_filename  = "given_filename.cpp";
    +        request.entry_filename  = "entry_filename";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +    }
    +
         return 0;
     }


### Commit 880062
% git checkout -q 880062  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 8800623e381c2cfe97f306c86e1a47abe6275c7f
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:03:17 2018 -0400
    
        assert filename
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index dd9e0d0..e4d81ae 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -54,7 +54,7 @@ int main() {
             request.option_language = "";
     
             auto filename = analysis_filename(request);
    -        assert(filename == "");
    +        assert(filename == "entry_filename");
             assert(analysis_url(request) == "");
             assert(analysis_language(request, filename) == "");
             assert(code_analysis(request) == false);


### Commit 7fee25
% git checkout -q 7fee25  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 7fee255564ef836d38fbca046a80de039e3e9ced
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:10:19 2018 -0400
    
        fix filename based on extension in filename analysis function
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 84f4894..43a409d 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -32,8 +32,13 @@ bool code_analysis(const analysis_request& request) {
      * @retval filename
      */
     std::string analysis_filename(const analysis_request& request) {
    -
    -    return request.option_filename;
    +    if (request.option_filename != "")
    +      return request.option_filename;
    +    std::string ext = get_language_from_filename(request.given_filename);
    +    if (ext == "")
    +      return request.given_filename;
    +    else
    +      return request.entry_filename;
     }
     
     /** URL extracted from the request


### Commit 91b20f
% git checkout -q 91b20f  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 91b20fca1b9f7bf7859c0c3c18b3e155c6a7e190
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:18:16 2018 -0400
    
        given_url failed test
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index e4d81ae..07c4c18 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -28,6 +28,7 @@ int main() {
             assert(code_analysis(request) == false);
         }
     
    +    // only option_filename filled in
         {
             analysis_request request;
             request.given_filename  = "";
    @@ -44,6 +45,7 @@ int main() {
             assert(code_analysis(request) == false);
         }
     
    +    // given_filename (as source-code file) and entry_filename filled in
         {
             analysis_request request;
             request.given_filename  = "given_filename.cpp";
    @@ -60,5 +62,39 @@ int main() {
             assert(code_analysis(request) == false);
         }
     
    +    // given_filename (as regular file) and entry_filename filled in
    +    {
    +        analysis_request request;
    +        request.given_filename  = "given_filename.txt";
    +        request.entry_filename  = "data";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "given_filename.txt");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +    }
    +
    +    // given_url filled in
    +    {
    +        analysis_request request;
    +        request.given_filename  = "";
    +        request.entry_filename  = "";
    +        request.given_url       = "given_url";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "");
    +        assert(analysis_url(request) == "given_url");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +    }
    +
         return 0;
     }


### Commit d3f942
% git checkout -q d3f942  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit d3f942ac400732ac0b4723659b39a15c7749cdec
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:19:17 2018 -0400
    
        given_url test passes
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 43a409d..f9ea999 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -47,7 +47,7 @@ std::string analysis_filename(const analysis_request& request) {
      */
     std::string analysis_url(const analysis_request& request) {
     
    -    return "";
    +    return request.given_url;
     }
     
     /** Language extracted from the request and the filename


### Commit 28c15a
% git checkout -q 28c15a  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 28c15ae09f419bba92c4824ba2342c4313dfe4ef
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:21:29 2018 -0400
    
        given_url and option_url failed test
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 07c4c18..6a9dafe 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -96,5 +96,22 @@ int main() {
             assert(code_analysis(request) == false);
         }
     
    +    // given_url and option_url filled in
    +    {
    +        analysis_request request;
    +        request.given_filename  = "";
    +        request.entry_filename  = "";
    +        request.given_url       = "given_url";
    +        request.option_filename = "";
    +        request.option_url      = "option_url";
    +        request.option_language = "";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "");
    +        assert(analysis_url(request) == "option_url");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +    }
    +
         return 0;
     }


### Commit b571e9
% git checkout -q b571e9  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit b571e99f3d4812a070a0926e98abab8bb00a2672
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:23:19 2018 -0400
    
        given/option url pass test
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index f9ea999..db4cfcc 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -46,7 +46,8 @@ std::string analysis_filename(const analysis_request& request) {
      * @retval URL
      */
     std::string analysis_url(const analysis_request& request) {
    -
    +    if (request.option_url != "")
    +      return request.option_url;
         return request.given_url;
     }
     


### Commit 42cb9c
% git checkout -q 42cb9c  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 42cb9c8fdb3781d51dc38babbf1e43970996f206
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:27:09 2018 -0400
    
        option_language test fail
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 6a9dafe..97c40e9 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -113,5 +113,22 @@ int main() {
             assert(code_analysis(request) == false);
         }
     
    +    // option_language filled in
    +    {
    +        analysis_request request;
    +        request.given_filename  = "";
    +        request.entry_filename  = "";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "option_language";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "option_language");
    +        assert(code_analysis(request) == false);
    +    }
    +
         return 0;
     }


### Commit 3a37f4
% git checkout -q 3a37f4  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 3a37f43628233c2f4bd36b7551fa1674f1a4c5ff
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:28:02 2018 -0400
    
        option_language pass test
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index db4cfcc..84a06c4 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -58,5 +58,5 @@ std::string analysis_url(const analysis_request& request) {
      */
     std::string analysis_language(const analysis_request& request, const std::string& filename) {
     
    -    return "";
    +    return request.option_language;
     }


### Commit 0b5e74
% git checkout -q 0b5e74  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 0b5e746a6fdb92c2e57cb935d5e70c3241143282
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Wed Apr 4 16:41:19 2018 -0400
    
        updated up to language analysis. relook at filename analysis. wrong defn of regular file v archive
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 84a06c4..d114229 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -57,6 +57,6 @@ std::string analysis_url(const analysis_request& request) {
      * @retval language
      */
     std::string analysis_language(const analysis_request& request, const std::string& filename) {
    -
    +    
         return request.option_language;
     }


### Commit a25f1c
% git checkout -q a25f1c  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit a25f1c9567c06bf8a3995cfd13dadb3149b02477
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Thu Apr 5 17:10:44 2018 -0400
    
        adjusted filename analysis
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index d114229..9038ab9 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -35,7 +35,7 @@ std::string analysis_filename(const analysis_request& request) {
         if (request.option_filename != "")
           return request.option_filename;
         std::string ext = get_language_from_filename(request.given_filename);
    -    if (ext == "")
    +    if (ext != "")
           return request.given_filename;
         else
           return request.entry_filename;
    @@ -57,6 +57,6 @@ std::string analysis_url(const analysis_request& request) {
      * @retval language
      */
     std::string analysis_language(const analysis_request& request, const std::string& filename) {
    -    
    +
         return request.option_language;
     }
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 97c40e9..8b3b6b8 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -48,7 +48,7 @@ int main() {
         // given_filename (as source-code file) and entry_filename filled in
         {
             analysis_request request;
    -        request.given_filename  = "given_filename.cpp";
    +        request.given_filename  = "given_filename.zip";
             request.entry_filename  = "entry_filename";
             request.given_url       = "";
             request.option_filename = "";
    @@ -65,7 +65,7 @@ int main() {
         // given_filename (as regular file) and entry_filename filled in
         {
             analysis_request request;
    -        request.given_filename  = "given_filename.txt";
    +        request.given_filename  = "given_filename.cpp";
             request.entry_filename  = "data";
             request.given_url       = "";
             request.option_filename = "";
    @@ -73,7 +73,7 @@ int main() {
             request.option_language = "";
     
             auto filename = analysis_filename(request);
    -        assert(filename == "given_filename.txt");
    +        assert(filename == "given_filename.cpp");
             assert(analysis_url(request) == "");
             assert(analysis_language(request, filename) == "");
             assert(code_analysis(request) == false);


### Commit 15c62f
% git checkout -q 15c62f  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 15c62fa22b22d50888d35679a07d16e216d6802d
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Thu Apr 5 17:28:50 2018 -0400
    
        test for stdin fail
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 9038ab9..2127c5e 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -37,8 +37,7 @@ std::string analysis_filename(const analysis_request& request) {
         std::string ext = get_language_from_filename(request.given_filename);
         if (ext != "")
           return request.given_filename;
    -    else
    -      return request.entry_filename;
    +    return request.entry_filename;
     }
     
     /** URL extracted from the request
    @@ -57,6 +56,7 @@ std::string analysis_url(const analysis_request& request) {
      * @retval language
      */
     std::string analysis_language(const analysis_request& request, const std::string& filename) {
    -
    -    return request.option_language;
    +    if (request.option_language != "")
    +      return request.option_language;
    +    return get_language_from_filename(request.given_filename);
     }
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 8b3b6b8..6254ed6 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -130,5 +130,22 @@ int main() {
             assert(code_analysis(request) == false);
         }
     
    +    // test for stdin for regular file and option_filename specified
    +    {
    +        analysis_request request;
    +        request.given_filename  = "-";
    +        request.entry_filename  = "data";
    +        request.given_url       = "";
    +        request.option_filename = "option_filename";
    +        request.option_url      = "";
    +        request.option_language = "";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "option_filename");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +    }
    +
         return 0;
     }


### Commit 8c2680
% git checkout -q 8c2680  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 8c268051f50be36a13f48825828753eb40ea627a
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Thu Apr 5 17:31:22 2018 -0400
    
        fixed error
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 6254ed6..8994b45 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -75,7 +75,7 @@ int main() {
             auto filename = analysis_filename(request);
             assert(filename == "given_filename.cpp");
             assert(analysis_url(request) == "");
    -        assert(analysis_language(request, filename) == "");
    +        assert(analysis_language(request, filename) == "C++");
             assert(code_analysis(request) == false);
         }
     


### Commit c0d872
% git checkout -q c0d872  
% make  

    make: Nothing to be done for `all'.

% git show

    commit c0d872e83752c5a7870d67ddeb777b86112bd776
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Thu Apr 5 17:39:49 2018 -0400
    
        fixed analysis_language tests. was testing for wrong thing
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 8994b45..1c6f212 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -34,14 +34,14 @@ int main() {
             request.given_filename  = "";
             request.entry_filename  = "";
             request.given_url       = "";
    -        request.option_filename = "option_filename";
    +        request.option_filename = "option_filename.cpp";
             request.option_url      = "";
             request.option_language = "";
     
             auto filename = analysis_filename(request);
    -        assert(filename == "option_filename");
    +        assert(filename == "option_filename.cpp");
             assert(analysis_url(request) == "");
    -        assert(analysis_language(request, filename) == "");
    +        assert(analysis_language(request, filename) == "C++");
             assert(code_analysis(request) == false);
         }
     
    @@ -121,12 +121,12 @@ int main() {
             request.given_url       = "";
             request.option_filename = "";
             request.option_url      = "";
    -        request.option_language = "option_language";
    +        request.option_language = "C++";
     
             auto filename = analysis_filename(request);
             assert(filename == "");
             assert(analysis_url(request) == "");
    -        assert(analysis_language(request, filename) == "option_language");
    +        assert(analysis_language(request, filename) == "C++");
             assert(code_analysis(request) == false);
         }
     
    @@ -136,16 +136,50 @@ int main() {
             request.given_filename  = "-";
             request.entry_filename  = "data";
             request.given_url       = "";
    -        request.option_filename = "option_filename";
    +        request.option_filename = "option_filename.cpp";
    +        request.option_url      = "";
    +        request.option_language = "";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "option_filename.cpp");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "C++");
    +        assert(code_analysis(request) == false);
    +    }
    +
    +    // test for stdin for archive file
    +    {
    +        analysis_request request;
    +        request.given_filename  = "-";
    +        request.entry_filename  = "entry_filename.zip";
    +        request.given_url       = "";
    +        request.option_filename = "";
             request.option_url      = "";
             request.option_language = "";
     
             auto filename = analysis_filename(request);
    -        assert(filename == "option_filename");
    +        assert(filename == "entry_filename.zip");
             assert(analysis_url(request) == "");
             assert(analysis_language(request, filename) == "");
             assert(code_analysis(request) == false);
         }
     
    +    // test for stdin with entry_filename == "data" and option_filename == ""
    +    {
    +        analysis_request request;
    +        request.given_filename  = "-";
    +        request.entry_filename  = "data";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "C++";
    +
    +        auto filename = analysis_filename(request);
    +        assert(filename == "");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "C++");
    +        assert(code_analysis(request) == false);
    +    }
    +
         return 0;
     }


### Commit a411fb
% git checkout -q a411fb  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit a411fb37aaa15b7a06fac815075fa20eeb59b87a
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Thu Apr 5 17:46:01 2018 -0400
    
        fixed analysis_language up to latest test
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 2127c5e..9bf015b 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -58,5 +58,5 @@ std::string analysis_url(const analysis_request& request) {
     std::string analysis_language(const analysis_request& request, const std::string& filename) {
         if (request.option_language != "")
           return request.option_language;
    -    return get_language_from_filename(request.given_filename);
    +    return get_language_from_filename(filename);
     }


### Commit 13faf2
% git checkout -q 13faf2  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 13faf25e00c0b223bbc9c4467b255b4300a17363
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Thu Apr 5 17:49:52 2018 -0400
    
        passes all tests
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 9bf015b..94d8c66 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -32,6 +32,8 @@ bool code_analysis(const analysis_request& request) {
      * @retval filename
      */
     std::string analysis_filename(const analysis_request& request) {
    +    if (request.given_filename == "-" && request.entry_filename == "data" && request.option_filename == "")
    +      return "";
         if (request.option_filename != "")
           return request.option_filename;
         std::string ext = get_language_from_filename(request.given_filename);


### Commit 8d5ca0
% git checkout -q 8d5ca0  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 8d5ca04fdd22c42f1712177ec6dac9db78f1c069
    Author: Steven Tyler Bissell <stb49@zips.uakron.edu>
    Date:   Thu Apr 5 18:03:28 2018 -0400
    
        added error handling
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 94d8c66..217baa2 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -8,6 +8,7 @@
     #include "get_language_from_filename.hpp"
     
     #include <string>
    +#include <iostream>
     
     /** Generate source analysis based on the request
      * @param request Data that forms the request
    @@ -24,6 +25,14 @@ bool code_analysis(const analysis_request& request) {
     
         // code analysis processing that is not yet implemented
     
    +    // error handling
    +    if (language == "") {
    +      std::cerr << "Extension not supported\n";
    +      if (request.given_filename == "-")
    +        std::cerr << "Using stdin requires a declared language\n";
    +    return false;
    +    }
    +
         return false;
     }
     

