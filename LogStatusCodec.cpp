#include<cinttypes>
#include<sag_connectivity_plugins.hpp>
#include<string>

using com::softwareag::connectivity::Logger;
using com::softwareag::connectivity::AbstractCodec;
using com::softwareag::connectivity::PluginConstructorParameters;
using com::softwareag::connectivity::AbstractSimpleCodec;
using com::softwareag::connectivity::Message;
using com::softwareag::connectivity::list_t;
using com::softwareag::connectivity::map_t;
using com::softwareag::connectivity::data_t;
using com::softwareag::connectivity::get;
using com::softwareag::connectivity::convert_to;

namespace com::apamax {

class LogStatusCodec: public AbstractSimpleCodec
{
public:
	explicit LogStatusCodec(const CodecConstructorParameters &param)
		: AbstractSimpleCodec(param)
	{}
	virtual bool transformMessageTowardsTransport(Message &m) override
	{
		// inbound only, blackhole everything outbound
		return false;
	}
	virtual bool transformMessageTowardsHost(Message &m) override
	{
		try {
			std::vector<Message> ms;
			auto &logcontents = get<const char*>(m.getPayload());
			const char *current, *old;
			current = old = logcontents;
			for (; *current; ++current) {
				if (*current == '\n') {
					std::string line(old, (current-old));
					parseMessageFromLine(line, ms, m.getMetadataMap());

					old=current;
				}
			}
			std::string line(old, (current-old));
			parseMessageFromLine(line, ms, m.getMetadataMap());
			hostSide->sendBatchTowardsHost(ms.begin(), ms.end());
			return false;
		} catch (const std::exception &e) {
			throw;
		}
	}
private:
	void parseMessageFromLine(const std::string &line, std::vector<Message> &ms, const map_t &meta)
	{
		size_t pos;
		if ((pos = line.find("Correlator Status:")) != std::string::npos) {
			map_t data;
			while ((pos=line.find('=', pos)) != std::string::npos) {
				auto namestart = line.rfind(' ', pos);
				if (namestart == std::string::npos) namestart = 0;
				auto name = line.substr(namestart+1, pos-namestart-1);
				std::string val;
				if (line[pos+1] == '"') {
					auto valend = line.find('"', pos+2);
					val = line.substr(pos+2, valend-pos-2);
				} else {
					auto valend = line.find(' ', pos+1);
					val = line.substr(pos+1, valend-pos-1);
				}
				data.insert(std::make_pair(name, val));
				pos++;
			}
			ms.push_back(Message(data_t(std::move(data)), meta.copy()));
		}
	}

};

SAG_DECLARE_CONNECTIVITY_CODEC_CLASS(LogStatusCodec)

} // com::apamax

