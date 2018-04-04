/*
  @file code_analysis.cpp

  Implementation of analysis requests
*/

#include "code_analysis.hpp"
#include "get_language_from_filename.hpp"

#include <string>

/** Generate source analysis based on the request
 * @param request Data that forms the request
 * @retval true   Valid request
 * @retval false  Invalid filename or unable to deduce language
 */
bool code_analysis(const analysis_request& request) {

    auto filename = analysis_filename(request);

    auto url = analysis_url(request);

    auto language = analysis_language(request, filename);

    // code analysis processing that is not yet implemented

    return false;
}

/** Filename extracted from the request
 * @param  request Data that forms the request
 * @retval filename
 */
std::string analysis_filename(const analysis_request& request) {

    return request.option_filename;
}

/** URL extracted from the request
 * @param  request Data that forms the request
 * @retval URL
 */
std::string analysis_url(const analysis_request& request) {

    return "";
}

/** Language extracted from the request and the filename
 * @param  request Data that forms the request
 * @param  filename Filename extracted from the request
 * @retval language
 */
std::string analysis_language(const analysis_request& request, const std::string& filename) {

    return "";
}
