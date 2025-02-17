#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/*
 * Complete the solve function below.
 */
int solve(vector<int> t) {
    /*
     * Return the ID
     */
	int students = -1;
	int starting_from = 1;
	for (int start = 0; start < t.size(); ++start) {
		int minutes = 0, current_students = 0;
	
		for (int i = start; i < t.size(); ++i, ++minutes) if (t[i] <= minutes) ++current_students;	
		for (int i = 0; i < start; ++i, ++minutes) if (t[i] <= minutes) ++current_students;

		if (current_students > students) {
			students = current_students;
			starting_from = start;
		}
	}

	return starting_from + 1;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t_count;
    cin >> t_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string t_temp_temp;
    getline(cin, t_temp_temp);

    vector<string> t_temp = split_string(t_temp_temp);

    vector<int> t(t_count);

    for (int t_itr = 0; t_itr < t_count; t_itr++) {
        int t_item = stoi(t_temp[t_itr]);

        t[t_itr] = t_item;
    }

    int id = solve(t);

    fout << id << "\n";

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}

