#include <string>
#include <map>
#include <vector>
#include <sstream>

namespace wpg
{
	namespace fileformats
	{
		class SimpleJSONListEntry
		{
		private:
			std::string escape(const std::string& str)
			{
				std::string escaped = "";
				for (auto& c : str)
				{
					if (c == '\\' || c == '\"')
						escaped += '\\'; // escape!
					escaped += c;
				}
				return escaped;
			}

		public:
			void SetValue(std::string key, unsigned long long value)
			{
				std::ostringstream ss;
				ss << value;
				entries[key] = ss.str();
			}

			void SetValue(std::string key, long long value)
			{
				std::ostringstream ss;
				ss << value;
				entries[key] = ss.str();
			}

			void SetValue(std::string key, const std::string& value)
			{
				std::string escaped = escape(value);

				std::ostringstream ss;
				ss << "\"" << escaped << "\"";
				entries[key] = ss.str();
			}

			std::string Build()
			{
				std::ostringstream ss;
				ss << "{";
				for (auto it = entries.begin(); it != entries.end(); it++)
				{
					ss << "\"" << it->first << "\": " << it->second;
					if (it != (--entries.end()))
						ss << ",";
				}
				ss << "}";
				return ss.str();
			}

			std::map<std::string, std::string> entries;
		};

		class SimpleJSONListBuilder
		{
		public:
			std::vector<std::string> entries;
			
			void AddEntry(SimpleJSONListEntry& entry)
			{
				entries.push_back(entry.Build());
			}

			std::string Build()
			{
				std::stringstream ss;
				ss << "[";
				for (auto it = entries.begin(); it != entries.end(); it++)
				{
					ss << *it;
					if (it != (--entries.end()))
						ss << ",";
				}
				ss << "]";
				return ss.str();
			}
		};
	}
}