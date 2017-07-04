#include <limits>
#include <algorithm>

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

	preprocess_types(d);

	generate_types(d);
	s_outf_types.output();

	generate_id(d);
	s_outf_id.output();

	generate_converter(d);
	if (0) {
		generate_msgs(d);
	}
	else {
		generate_wsmsgs(d);
	}
	s_outf_converter.output();
	s_outf_sender.output();
	s_outf_rcver.output();

	return 0;
}
