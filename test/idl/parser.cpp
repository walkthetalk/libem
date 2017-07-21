#include <limits>
#include <algorithm>
#include <iostream>

#include "parser.hpp"
#include "filebuf.hpp"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>


int main(int /*argc*/, char * /*argv*/[])
{
	std::ifstream ifs(IDLFILE);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document d;
	d.ParseStream<rapidjson::kParseTrailingCommasFlag>(isw);

	std::cout << "step 1: preprocess" << std::endl;
	preprocess_types(d);

	std::cout << "step 2: generate types" << std::endl;
	generate_types(d);
	s_outf_types.output();

	std::cout << "step 3: generate id" << std::endl;
	generate_id(d);
	s_outf_id.output();

	std::cout << "step 4: generate converter" << std::endl;
	generate_converter(d);
	std::cout << "step 5: generate message" << std::endl;
	generate_wsmsgs(d);
	std::cout << "step 6: generate for pqxx" << std::endl;
	generate_pqxx_converter(d);
	s_outf_converter.output();
	s_outf_pqxx_hpp.output();
	s_outf_pqxx_converter.output();
	s_outf_sender.output();
	s_outf_rcver.output();

	return 0;
}
