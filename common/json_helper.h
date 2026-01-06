/*
 * JSON Helper - Simple JSON parsing/building without external libraries
 * Hỗ trợ parse và build JSON cho protocol checkpoint 1
 */

#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <string>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

class JsonHelper {
public:
    // Escape special characters for JSON string
    static string escapeJson(const string& str) {
        string result;
        result.reserve(str.length() * 2);
        for (char c : str) {
            switch (c) {
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default: result += c;
            }
        }
        return result;
    }
    
    // Parse JSON string thành map
    static map<string, string> parse(const string& json_str) {
        map<string, string> result;
        
        // Bỏ { và }
        string content = json_str;
        size_t start = content.find('{');
        size_t end = content.rfind('}');
        if (start == string::npos || end == string::npos) return result;
        
        content = content.substr(start + 1, end - start - 1);
        
        // Parse các cặp "key":"value"
        size_t pos = 0;
        while (pos < content.length()) {
            // Tìm key
            size_t key_start = content.find('"', pos);
            if (key_start == string::npos) break;
            
            size_t key_end = content.find('"', key_start + 1);
            if (key_end == string::npos) break;
            
            string key = content.substr(key_start + 1, key_end - key_start - 1);
            
            // Tìm value
            size_t value_start = content.find('"', key_end + 1);
            if (value_start == string::npos) {
                // Thử tìm giá trị số hoặc boolean
                size_t colon = content.find(':', key_end);
                size_t comma = content.find(',', colon);
                if (comma == string::npos) comma = content.length();
                
                string value = content.substr(colon + 1, comma - colon - 1);
                // Trim spaces
                value.erase(0, value.find_first_not_of(" \t\n\r"));
                value.erase(value.find_last_not_of(" \t\n\r") + 1);
                
                result[key] = value;
                pos = comma + 1;
                continue;
            }
            
            size_t value_end = content.find('"', value_start + 1);
            if (value_end == string::npos) break;
            
            string value = content.substr(value_start + 1, value_end - value_start - 1);
            result[key] = value;
            
            pos = value_end + 1;
        }
        
        return result;
    }
    
    // Build JSON string từ map
    static string build(const map<string, string>& data) {
        if (data.empty()) return "{}";
        
        stringstream ss;
        ss << "{";
        
        bool first = true;
        for (const auto& pair : data) {
            if (!first) ss << ",";
            ss << "\"" << pair.first << "\":\"" << pair.second << "\"";
            first = false;
        }
        
        ss << "}";
        return ss.str();
    }
    
    // Parse JSON array
    static vector<string> parse_array(const string& json_str, const string& key) {
        vector<string> result;
        
        // Tìm key
        size_t key_pos = json_str.find("\"" + key + "\"");
        if (key_pos == string::npos) return result;
        
        // Tìm [
        size_t arr_start = json_str.find('[', key_pos);
        if (arr_start == string::npos) return result;
        
        // Tìm ]
        size_t arr_end = json_str.find(']', arr_start);
        if (arr_end == string::npos) return result;
        
        string array_content = json_str.substr(arr_start + 1, arr_end - arr_start - 1);
        
        // Parse các phần tử
        size_t pos = 0;
        while (pos < array_content.length()) {
            size_t start = array_content.find('"', pos);
            if (start == string::npos) break;
            
            size_t end = array_content.find('"', start + 1);
            if (end == string::npos) break;
            
            result.push_back(array_content.substr(start + 1, end - start - 1));
            pos = end + 1;
        }
        
        return result;
    }
    
    // Build JSON với array
    static string build_with_array(const map<string, string>& data, 
                                    const string& array_key, 
                                    const vector<string>& array_values) {
        stringstream ss;
        ss << "{";
        
        bool first = true;
        for (const auto& pair : data) {
            if (!first) ss << ",";
            ss << "\"" << pair.first << "\":\"" << pair.second << "\"";
            first = false;
        }
        
        if (!array_values.empty()) {
            if (!first) ss << ",";
            ss << "\"" << array_key << "\":[";
            for (size_t i = 0; i < array_values.size(); i++) {
                if (i > 0) ss << ",";
                ss << "\"" << array_values[i] << "\"";
            }
            ss << "]";
        }
        
        ss << "}";
        return ss.str();
    }
};

#endif // JSON_HELPER_H
