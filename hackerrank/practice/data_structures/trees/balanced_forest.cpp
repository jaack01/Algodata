#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

class Forest;
class WeightForest;


class Forest
{
protected:
    list<Forest *> children{};
    Forest *parent{};
    int value{};

public:
    Forest(int value, Forest *parent = nullptr, list<Forest *> children = list<Forest *>{})
        : value{value}, children{children}, parent{parent} {}
    Forest() = default;
    ~Forest()
    {
        for (auto child : children)
            delete child;
    }

    void preorder() const
    {
        cout << value << ' ';
        for (auto child : children)
            child->postorder();
    }

    void postorder() const
    {
        for (auto child : children)
            child->postorder();
        cout << value << ' ';
    }

    void setValue(int val)
    {
        value = val;
    }

    int getValue()
    {
        return value;
    }

    void addChild(int value)
    {
        children.push_back(new Forest{value});
        children.back()->parent = this;
    }

    void addChild(Forest *child)
    {
        children.push_back(child);
        child->parent = this;
    }

    static Forest constructFrom(vector<int> c, vector<vector<int>> edges)
    {
        Forest root{};
        // adding dummy 0-value node to root,
        // so that we had at least 3 nodes in the tree
        c.push_back(0);
        edges.push_back(vector<int>{1, c.size()});
        _constructFrom(c, edges, &root);
        return root;
    }

private:
    friend void _constructFrom(const vector<int> &c, const vector<vector<int>> &edges, Forest *root, Forest *parent = nullptr, int i = 1)
    {
        root->value = c[i - 1];
        root->parent = parent;
        for (auto edge : edges)
            if (edge[0] == i)
            {
                // push back new child
                root->addChild(edge[1]);
                _constructFrom(c, edges, root->children.back(), root, edge[1]);
            }
    }
};

class WeightForest : public Forest
{
public:
    WeightForest(const Forest *root)
    {
        const WeightForest* wroot = static_cast<const WeightForest*>(root);
        wroot = _weightsTree(wroot);
        this->value = wroot->value;
        this->children = wroot->children;
        this->parent = wroot->parent;
    }

    WeightForest(int value, Forest *parent = nullptr, list<Forest *> children = list<Forest *>{})
        : Forest(value, parent, children) {}

    // friend pair<unique_ptr<Forest>, list<Forest *>::iterator> cutWeightNode(Forest *parent, list<Forest *>::iterator child_it)
    // {
    //     Forest *current = *child_it;
    //     int cut_weight = current->value;
    //     while (current->parent != nullptr)
    //     {
    //         current = current->parent;
    //         current->value -= cut_weight;
    //     }
    //     current = *child_it;
    //     auto next = parent->children.erase(child_it);

    //     return {unique_ptr<Forest>{current}, next};
    // }

    // friend void insertWeightNode(list<Forest *>::iterator child_it, list<Forest *>::iterator after_child_it)
    // {
    //     Forest *current = *child_it;
    //     Forest *parent = *affter_child_it->parent;
    //     parent->children.insert(after_child_it, child_it);
    //     int insert_weight = current->value;
    //     while (parent != nullptr)
    //     {
    //         parent->value += insert_weight;
    //         parent = parent->parent;
    //     }
    // }

private:
    static WeightForest *_weightsTree(const WeightForest *root, WeightForest *parent = nullptr)
    {
        WeightForest *weight_node = new WeightForest{root->value, static_cast<Forest*>(parent)};
        for (auto child : root->children)
        {
            WeightForest *child_sum = _weightsTree(static_cast<WeightForest*>(child), weight_node);
            weight_node->addChild(child_sum);
            weight_node->value += child_sum->value;
        }

        return weight_node;
    }
};

int main()
{
    Forest root = Forest::constructFrom(vector<int>{15, 12, 8, 14, 13}, vector<vector<int>>{{1, 2}, {1, 3}, {1, 4}, {4, 5}});
    WeightForest wroot{&root};
    root.postorder();
    cout << '\n';
    wroot.postorder();
    cout << '\n';

    return 0;
}

// // Complete the balancedForest function below.
// int balancedForest(vector<int> &&c, vector<vector<int>> &&edges)
// {
//     Forest wroot = Forest::constructFrom(std::move(c), std::move(edges));
//     return wroot.balancing();
// }

// vector<string> split_string(string input_string)
// {
//     string::iterator new_end = unique(input_string.begin(), input_string.end(), [](const char &x, const char &y) {
//         return x == y and x == ' ';
//     });

//     input_string.erase(new_end, input_string.end());

//     while (input_string[input_string.length() - 1] == ' ')
//     {
//         input_string.pop_back();
//     }

//     vector<string> splits;
//     char delimiter = ' ';

//     size_t i = 0;
//     size_t pos = input_string.find(delimiter);

//     while (pos != string::npos)
//     {
//         splits.push_back(input_string.substr(i, pos - i));

//         i = pos + 1;
//         pos = input_string.find(delimiter, i);
//     }

//     splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

//     return splits;
// }

// int main()
// {
//     ofstream fout(getenv("OUTPUT_PATH"));

//     int q;
//     cin >> q;
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');

//     for (int q_itr = 0; q_itr < q; q_itr++)
//     {
//         int n;
//         cin >> n;
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');

//         string c_temp_temp;
//         getline(cin, c_temp_temp);

//         vector<string> c_temp = split_string(c_temp_temp);

//         vector<int> c(n);

//         for (int i = 0; i < n; i++)
//         {
//             int c_item = stoi(c_temp[i]);

//             c[i] = c_item;
//         }

//         vector<vector<int>> edges(n - 1);
//         for (int i = 0; i < n - 1; i++)
//         {
//             edges[i].resize(2);

//             for (int j = 0; j < 2; j++)
//             {
//                 cin >> edges[i][j];
//             }

//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         }

//         int result = balancedForest(std::move(c), std::move(edges));

//         cout << result << "\n";
//         fout << result << "\n";
//     }

//     fout.close();

//     return 0;
// }
