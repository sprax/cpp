class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string result;
        if (strs.size()) {
            const string& common = strs[0];
            int sz = common.length();
            int strs_size = strs.size();
            for (int h = 1; h < strs_size; h++) {
                const auto& str = strs[h];
                for (int j = 0; j < sz; j++) {
                    if (str[j] != common[j]) {
                        sz = j;
                        break;
                    }
                }
            }
            if (sz) {
                result = common.substr(0, sz);
            }
        }
        return result;
    }
};
