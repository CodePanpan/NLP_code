/** Include Header Files */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include <stack>
#include <queue>
/** Header Files Included */

/** Database Sizing */
#define NoOfOperatorWords 29
#define NoOfActions 5
#define NoOfVariables 22
#define NoOfLocations 3
#define NoOfTimes 9
#define NoOfNXCmds 7
#define NoOfAllEachEveryKeys 3
#define NoOfExceptionalOperators 1
/** Database Sizing Complete */

/** Default Value Variables */
#define DefaultComponent "Unknown"
#define DefaultAction "Unknown"
#define DefaultLocation "node"
#define DefaultWhen "time"
#define CombineAcrossEntitiesDefault false
#define CombineAcrossIterationsDefault false
#define CombineOptionEntitiesDefault "Arithmetic"
#define CombineOptionIterationsDefault "Arithmetic"
#define DefaultWhenValue "0"
#define DefaultLocationValue "entire model"
#define CommaReplacingAnd " and"
#define Abnormal "abnormal"
#define BlankSpace " "
#define ToKeyword " to "
#define AllKeyword "all"
#define EntireKeyword "entire"
#define EmptyString ""
#define OpenBracket "("
#define ClosedBracket ")"
//Operator Defaults
#define PlusOperator "+"
#define MinusOperator "-"
#define MultiplyOperator "*"
#define DivideOperator "/"
#define MaximumOperator "m"
#define MinimumOperator "n"
#define AverageOperator "g"
/** Default Value Variables Set */

using namespace std;

/** Global Variables */
/** Operators */
string operators[NoOfOperatorWords][2] = {
    {"Add", PlusOperator},
    {"Addition", PlusOperator},
    {"Added", PlusOperator},
    {"Sum", PlusOperator},
    {"Summation", PlusOperator},
    {"Summed", PlusOperator},
    {"Plus", PlusOperator},
    {"Subtract", MinusOperator},
    {"Subtraction", MinusOperator},
    {"Subtracted", MinusOperator},
    {"Difference", MinusOperator},
    {"Minus", MinusOperator},
    {"Deduct", MinusOperator},
    {"Multiply", MultiplyOperator},
    {"Multiplication", MultiplyOperator},
    {"Multiplied", MultiplyOperator},
    {"Product", MultiplyOperator},
    {"Divide", DivideOperator},
    {"Division", DivideOperator},
    {"Divide", DivideOperator},
    {"Divided", DivideOperator},
    {"Upon", DivideOperator},
    {"Maximum", MaximumOperator},
    {"Max", MaximumOperator},
    {"Minimum", MinimumOperator},
    {"Min", MinimumOperator},
    {"Average", AverageOperator},
    {"Averaged", AverageOperator},
    {"Mean", AverageOperator}
};
/** Operators End */
/** Exceptional Operators Where Order Of Operands Is To Be Reversed */
string exceptional_operators[] = {
    "Difference"
};
/** Exceptional Operators End */
/** Actions */
string actions[] = {
    "Displacement",
    "Stress",
    "Rotation",
    "Temperature",
    "Reaction-Force",
    "Reaction-Moment"
};
/** Actions End */
/** Variables */
string variables[] = {
    "X",
    "Y",
    "Z",
    "Magnitude",
    "XX",
    "YY",
    "ZZ",
    "XY",
    "YX",
    "YZ",
    "ZY",
    "ZX",
    "XZ",
    "Determinant",
    "Mean",
    "Max-Shear",
    "Min-Principal",
    "Mid-Principal",
    "Max-Principal",
    "Worst-Principal",
    "Octahedral",
    "Von-Mises"
};
/** Variables End */
/** Locations */
string locations[] = {
    "Node",
    "Element",
    "Element-Node"
};
/** Location End */
/** Time */
string times[] = {
    "Time",
    "Timestep",
    "Timesteps",
    "Iteration",
    "Iterations",
    "second",
    "seconds",
    "sec",
    "secs"
};
/** Time End */
/** Across All Iterations Keywords */
string across_all_every[] = {
    "all",
    "every",
    "each"
};
/** Across All Iterations Keywords End */
/** NX Command Words */
string nx_cmd[] = {
    "Plot",
    "Graph",
    "Identify",
    "Find",
    "PostView",
    "Show",
    "Display"
};
/** NX Command Words End */
/** Supplementary Functions Declarations */
class Operand;
bool IsPresent(string t[], int sz, string srch);
int IsPresentInOperators(string srch);
int IsWhichOperator(string s);
bool IsExceptionalOperator(string s);
bool IsPresentInDatabase(string arr[], int sz, string srch);
bool IsAction(string s);
bool IsVariable(string s);
bool IsLocation(string s);
bool IsTime(string s);
bool IsPresentInAllEachEveryTimes(string s);
bool IsNXCmd(string s);
bool IsPartOfOperand(string s);
bool IsArrayBoundReached(int sz, int ind);
bool IsStringANumber(string s);
string ReplaceCommaBySpaceAnd(string s);
string *SetRange(vector<string> words, int i);
void CheckForAbnormalOperators(string op, int ind, vector<string> &words, stack<string> &bucket);
void SetNXCmd(string word, string &nx_cmd);
void ResetCurrentOperand(Operand *&currod,
                       string variable = DefaultComponent,
                       string action = DefaultAction,
                       string time = DefaultWhen,
                       string location = DefaultLocation,
                       string *l_r = NULL,
                       string *t_r = NULL);
void FillTheBucket(Operand *&currod, stack<string> &bucket);
bool FixMissingEntriesAndFillTheBucket(queue<Operand*> &ods, stack<string> &bucket);
void PushBracketBackWhenOperator(register int &bracket_ct, stack<string> &bucket);
void TreeConverter(stack<string> &bucket);
struct tree_expression;
void CreationOfChildren(stack<string> &tree_next_bucket, string &first, string &last, string &second_last, tree_expression *root);
bool IsPresent(string t[], int sz, string srch) {
    for (register int j = 0; j < sz; j++) {
        if(_stricmp(t[j].c_str(), srch.c_str()) == 0)
            return 1;
    }
    return 0;
}
int IsPresentInOperators(string srch) {
    for (register int i = 0; i < NoOfOperatorWords; i++) {
        if(_stricmp(operators[i][0].c_str(), srch.c_str()) == 0)
            return i;
    }
    return -1;
}
int IsWhichOperator(string s) {
    if ((_stricmp(s.c_str(), PlusOperator) == 0) ||
        (_stricmp(s.c_str(), MultiplyOperator) == 0))
        return 100;
    if ((_stricmp(s.c_str(), MinusOperator) == 0) ||
        (_stricmp(s.c_str(), DivideOperator) == 0))
            return 2;
    return 1;
}
bool IsExceptionalOperator(string s) {
    return IsPresentInDatabase(exceptional_operators, NoOfExceptionalOperators, s);
}
bool IsPresentInDatabase(string arr[], int sz, string srch) {
    for (register int i = 0; i < sz; i++)
        if(_stricmp(arr[i].c_str(), srch.c_str()) == 0)
            return 1;

    return 0;
}
bool IsPartOfOperand(string s) {
    if (IsAction(s) || (IsVariable(s)) || (IsLocation(s)) || (IsTime(s)))
        return 1;
    return 0;
}
bool IsAction(string s) {
    return IsPresentInDatabase(actions, NoOfActions, s);
}
bool IsVariable(string s) {
    return IsPresentInDatabase(variables, NoOfVariables, s);
}
bool IsLocation(string s) {
    return IsPresentInDatabase(locations, NoOfLocations, s);
}
bool IsTime(string s) {
    return IsPresentInDatabase(times, NoOfTimes, s);
}
bool IsPresentInAllEachEveryTimes(string s) {
    return IsPresentInDatabase(across_all_every, NoOfAllEachEveryKeys, s);
}
bool IsNXCmd(string s) {
    return IsPresentInDatabase(nx_cmd, NoOfNXCmds, s);
}
bool IsArrayBoundReached(int sz, int ind) {
    return (ind >= sz) ? 1 : 0;
}
bool IsStringANumber(string s) {
    istringstream inpStream(s);
    double inpValue = 0.0;
    return (inpStream >> inpValue) ? 1 : 0;
}
string ReplaceCommaBySpaceAnd(string s) {
    string new_str = EmptyString;
    string space_and = CommaReplacingAnd;
    string tmp = EmptyString;
    char ss[2];
    register int i = 0;
    while (s[i] != '\0') {
        if (s[i] == ',') {
            new_str.append(space_and);
        } else {
            ss[0] = s[i];ss[1] = '\0';
            tmp = ss;
            new_str.append(tmp);
        }
        i++;
    }
    return new_str;
}
string *SetRange(vector<string> words, int i) {
    string *l_r = new string[2];
    l_r[0] = EmptyString; l_r[1] = EmptyString;
    for (register int j = 1; j <= 5; j++) { //j should be 1 to 5. but this fails for from. Fix this later.
        if (!IsArrayBoundReached(words.size(), i+j)) {
            if ((IsLocation(words[i+j]) || (IsTime(words[i+j]))))
                break;
            if (IsStringANumber(words[i+j])) {
                if (l_r[0] == EmptyString) {
                    l_r[0] = words[i+j];
                }
                else if (l_r[1] == EmptyString) {
                    l_r[1] = words[i+j];
                }
            }
        } else
            break;
    }
    return l_r;
}
void CheckForAbnormalOperators(string op, int ind, vector<string> &words, stack<string> &bucket) {
    string from = "from", between = "between";
    string abnormal_flag = Abnormal;
    if (op == MinusOperator) {
        if ((ind+1) < words.size()) {
            if ((_stricmp(words[ind+1].c_str(), from.c_str()) == 0) ||
                (_stricmp(words[ind+1].c_str(), between.c_str()) == 0) ||
                (IsExceptionalOperator(words[ind]))) {
                    bucket.push(BlankSpace);
                    bucket.push(abnormal_flag);
            }
        }
    }
}
void SetNXCmd(string word, string &nx_cmd) {
    if (IsNXCmd(word))
        nx_cmd = word;
}
/** Supplementary Functions Declarations End */
/** Operand Class */
class Operand {
    string action;
    string variable;
    string time;
    string location;
    string *time_range;
    string *node_range;
public:
    Operand(string act = DefaultAction, string var = DefaultComponent, string *t_r = NULL, string *n_r = NULL, string t = DefaultWhen, string loc = DefaultLocation) {
        action = act;
        variable = var;
        time = t;
        location = loc;
        time_range = t_r;
        node_range = n_r;
    }
    Operand (const Operand &cp) {
        action = cp.GetAction();
        variable = cp.GetVariable();
        time = cp.GetTime();
        location = cp.GetLocation();
        if (cp.GetTimeRange() != NULL) {
            time_range = new string[2];
            time_range[0] = cp.GetTimeRange()[0];
            time_range[1] = cp.GetTimeRange()[1];
        } else
            time_range = NULL;
        if (cp.GetNodeRange() != NULL) {
            node_range = new string[2];
            node_range[0] = cp.GetNodeRange()[0];
            node_range[1] = cp.GetNodeRange()[1];
        } else
            node_range = NULL;
    }
    ~Operand() {
        delete[] time_range;
        delete[] node_range;
    }
    //Mutators
    void SetAction(string act) {action = act;}
    void SetVariable(string var) {variable = var;}
    void SetTime(string t) {time = t;}
    void SetLocation(string loc) {location = loc;}
    void SetTimeRange(string *t_r) {time_range = t_r;}
    void SetNodeRange(string *n_r) {node_range = n_r;}
    //Accessors
    string GetAction() const {return action;}
    string GetVariable () const {return variable;}
    string GetTime() const {return time;}
    string GetLocation() const {return location;}
    string *GetTimeRange() const {return time_range;}
    string *GetNodeRange() const {return node_range;}
};
/** Operand Class End */
void ResetCurrentOperand(Operand *&currod,
                       string variable,
                       string action,
                       string time,
                       string location,
                       string *l_r,
                       string *t_r) {
    currod->SetVariable(variable);
    currod->SetAction(action);
    currod->SetTime(time);
    currod->SetLocation(location);
    currod->SetTimeRange(t_r);
    currod->SetNodeRange(l_r);
}
void FillTheBucket(Operand *&currod, stack<string> &bucket) {
    string when = BlankSpace, loc = BlankSpace;
    when.append(currod->GetTime());when.append(BlankSpace);
    loc.append(currod->GetLocation()); loc.append(BlankSpace);
    if (currod->GetTimeRange() != NULL) {
    when.append(currod->GetTimeRange()[0]);
        if (currod->GetTimeRange()[1] != EmptyString) {
            when.append(ToKeyword);
            when.append(currod->GetTimeRange()[1]);
        }
    } else {
        when.append(DefaultWhenValue);
    }
    if (currod->GetNodeRange() != NULL) {
        loc.append(currod->GetNodeRange()[0]);
        if (currod->GetNodeRange()[1] != EmptyString) {
            loc.append(ToKeyword);
            loc.append(currod->GetNodeRange()[1]);
        }
    } else {
        loc.append(DefaultLocationValue);
    }
    bucket.push(BlankSpace);
    bucket.push(when);
    bucket.push(loc);
    bucket.push(currod->GetAction());
    bucket.push(BlankSpace);
    bucket.push(currod->GetVariable());
}
bool FixMissingEntriesAndFillTheBucket(queue<Operand*> &ods, stack<string> &bucket) {
    //Fix missing entries
    queue<Operand*> temp;
    register bool inside = 0;
    temp = ods;
    while (ods.size() != 1)
        ods.pop();
    Operand *tmpod = new Operand(*(ods.front()));
    string *rt = tmpod->GetTimeRange();
    string *rl = tmpod->GetNodeRange();
    string tact = tmpod->GetAction();
    string rtime = tmpod->GetTime();
    string rloc = tmpod->GetLocation();
    ods = temp;
    //pop entire queue
    string when = BlankSpace, loc = BlankSpace;
    while (ods.size() != 0) {
        when = BlankSpace, loc = BlankSpace;
        Operand *t = new Operand(*(ods.front()));
        when.append(t->GetTime());when.append(BlankSpace);
        loc.append(t->GetLocation()); loc.append(BlankSpace);
        if (t->GetTimeRange() == NULL) {
            t->SetTimeRange(rt);
        }
        if (t->GetNodeRange() == NULL)
            t->SetNodeRange(rl);
        if (t->GetAction() == DefaultAction)
            t->SetAction(tact);
        if (t->GetTime() == DefaultWhen)
            t->SetTime(rtime);
        if (t->GetLocation() == DefaultLocation)
            t->SetLocation(rloc);
        if (t->GetTimeRange() != NULL) {
            when.append(t->GetTimeRange()[0]);
            if (t->GetTimeRange()[1] != EmptyString) {
                when.append(ToKeyword);
                when.append(t->GetTimeRange()[1]);
            }
        } else {
            when.append(DefaultWhenValue);
        }
        if (t->GetNodeRange() != NULL) {
            loc.append(t->GetNodeRange()[0]);
            if (t->GetNodeRange()[1] != EmptyString) {
                loc.append(ToKeyword);
                loc.append(t->GetNodeRange()[1]);
            }
        } else {
            loc.append(DefaultLocationValue);
        }
        if ((t->GetVariable() != DefaultComponent) || (t->GetAction() != DefaultAction)) {
            inside = 1;
            bucket.push(BlankSpace);
            bucket.push(when);
            bucket.push(loc);
            bucket.push(t->GetAction());
            bucket.push(BlankSpace);
            bucket.push(t->GetVariable());
        }
        ods.pop();
    }
    return inside;
}
void PushBracketBackWhenOperator(register int &bracket_ct, stack<string> &bucket) {
    //Bracket resolution
    stack<string> bracket_resolution;
    register int b_ct = 1;
    bracket_resolution.push(bucket.top());bucket.pop();
    bracket_resolution.push(bucket.top());bucket.pop();
    while(b_ct != 0) {
        if (bucket.top() == OpenBracket)
            b_ct++;
        else if (bucket.top() == ClosedBracket)
            b_ct--;
        bracket_resolution.push(bucket.top());bucket.pop();
    }
    bucket.push(ClosedBracket);bracket_ct++;
    while(bracket_resolution.size() != 0) {
        bucket.push(bracket_resolution.top());
        bracket_resolution.pop();
    }
    //Bracket resolution is done
}
void SetVariable(string word,
                 Operand *&currod,
                 queue<Operand*> &ods,
                 stack<string> &bucket,
                 int *currop,
                 int *opcount,
                 register int &bracket_ct,
                 register int &currop_ind) {
    if (currod->GetVariable() == DefaultComponent) {
        currod->SetVariable(word);
    } else {
        Operand *od = new Operand(*currod);
        if (od->GetVariable() != DefaultComponent) {
            ods.push(od);
            opcount[currop_ind] += 1;
            if (opcount[currop_ind] == currop[currop_ind]) {
                FixMissingEntriesAndFillTheBucket(ods, bucket);
                bucket.push(OpenBracket);currop_ind--;opcount[currop_ind] += 1;bracket_ct--;
            }
        }
        ResetCurrentOperand(currod, word, DefaultAction, DefaultWhen, DefaultLocation, NULL, NULL);
    }
}
void SetAction(string word,
               Operand *&currod,
               queue<Operand*> &ods,
               stack<string> &bucket,
               int *currop,
               int *opcount,
               register int &bracket_ct,
               register int &currop_ind) {
    if (currod->GetAction() == DefaultAction) {
        currod->SetAction(word);
    } else  {
        Operand *od = new Operand(*currod);
        if (od->GetAction() != DefaultAction) {
            ods.push(od);  //Look into this
            opcount[currop_ind] += 1;
            if (opcount[currop_ind] == currop[currop_ind]) {
                FixMissingEntriesAndFillTheBucket(ods, bucket);
                bucket.push(OpenBracket);currop_ind--;opcount[currop_ind] += 1;bracket_ct--;
            }
        }
        ResetCurrentOperand(currod, DefaultComponent, word, DefaultWhen, DefaultLocation, NULL, NULL);
    }
}
void SetLocation(string word, Operand *&currod, vector<string> words, int index) {
    currod->SetLocation(word);
    currod->SetNodeRange(SetRange(words, index));
}
void SetTime(string word, Operand *&currod, vector<string> words, int index) {
    currod->SetTime(word);
    currod->SetTimeRange(SetRange(words, index));
    if (currod->GetTimeRange()[0] == EmptyString) {
        if ((index - 1) >= 0) {
            if (IsPresentInAllEachEveryTimes(words[index-1])) {
                string *t_r = new string[2];
                t_r[0] = AllKeyword; t_r[1] = EmptyString;
                currod->SetTimeRange(t_r);
            }
        }
    }
}
struct tree_expression {
    bool IsOperator;
    vector<tree_expression *>operands_list;
    string tree_op;
    string tree_action;
    string tree_variable;
    string tree_location;
    string tree_location_value;
    string tree_time;
    string tree_time_value;
    tree_expression() {
        IsOperator = true;
        operands_list.clear();
        tree_op = "";
        tree_action = "";
        tree_variable = "";
        tree_location = "";
        tree_location_value = "";
        tree_time = "";
        tree_time_value = "";
    }
};
void SplitLocTimeData(string str, pair<string, string> &res) {
    string first = "", second = "";
    int i;
    if (str[0] == ' ')
        i = 1;
    else
        i = 0;
    while (str[i] != ' ') {
        first += str[i++];
    }
    first += '\0';
    i++;
    while (str[i] != '\0') {
        second += str[i++];
    }
    second += '\0';
    res.first = first;
    res.second = second;
}
void CorrectLocationAndTime(string &loc, string &loc_value, string &time, string &time_value) {
    pair<string, string> result;
    SplitLocTimeData(loc, result);
    loc = result.first;
    loc_value = result.second;

    SplitLocTimeData(time, result);
    time = result.first;
    time_value = result.second;
}
void ReverseStack(stack<string> &stck) {
    stack<string> temp;
    while (!stck.empty()) {
        temp.push(stck.top());
        stck.pop();
    }
    stck = temp;
}
void TreeInitialSetup(stack<string> &tree_bucket, stack<string> &tree_next_bucket, string &first, string &first_next, string &last, string &second_last) {
    //Initial setup
    tree_bucket.pop();
    first = tree_bucket.top();
    tree_next_bucket.push(tree_bucket.top());
    tree_bucket.pop();
    if (!tree_bucket.empty()) {
        tree_next_bucket.push(tree_bucket.top());
        tree_bucket.pop();
        first_next = tree_bucket.top();
        tree_next_bucket.push(tree_bucket.top());
        tree_bucket.pop();
    }
    while (tree_bucket.size() != 1) {
        if (tree_bucket.size() == 4) {
            second_last = tree_bucket.top();
            string abnormal_flag = Abnormal;
            if (_stricmp(second_last.c_str(), abnormal_flag.c_str()) != 0) {
                tree_next_bucket.push(tree_bucket.top());
                tree_bucket.pop();
                tree_next_bucket.push(tree_bucket.top());
                tree_bucket.pop();
                last = tree_bucket.top();
                tree_bucket.pop();
                break;
            } else {
                tree_bucket.pop();
                tree_bucket.pop();
                last = tree_bucket.top();
                tree_bucket.pop();
                break;
            }
        }
        tree_next_bucket.push(tree_bucket.top());
        tree_bucket.pop();
    }
    ReverseStack(tree_next_bucket);
}
void NoOperatorAnywhere(stack<string> &tree_next_bucket, tree_expression *root) {
    //No operator anywhere
    root->IsOperator = false;
    root->operands_list.clear();
    root->tree_variable = tree_next_bucket.top();
    tree_next_bucket.pop();
    tree_next_bucket.pop();
    root->tree_action = tree_next_bucket.top();
    tree_next_bucket.pop();
    root->tree_location = tree_next_bucket.top();
    tree_next_bucket.pop();
    root->tree_time = tree_next_bucket.top();
    tree_next_bucket.pop();
    if (!tree_next_bucket.empty())
        tree_next_bucket.pop();

    //Correcting location and time
    CorrectLocationAndTime(root->tree_location, root->tree_location_value, root->tree_time, root->tree_time_value);
}
void EntireBucketIsOperands(stack<string> &tree_bucket_next, tree_expression *root) {
    while (tree_bucket_next.top() == " ") {
        tree_bucket_next.pop();
    }
    while (!tree_bucket_next.empty()) {
        tree_expression *tree_operand = new tree_expression();
        tree_operand->IsOperator = false;
        tree_operand->tree_variable = tree_bucket_next.top();
        tree_bucket_next.pop();
        tree_bucket_next.pop();
        tree_operand->tree_action = tree_bucket_next.top();
        tree_bucket_next.pop();
        tree_operand->tree_location = tree_bucket_next.top();
        tree_bucket_next.pop();
        tree_operand->tree_time = tree_bucket_next.top();
        tree_bucket_next.pop();
        if (!tree_bucket_next.empty())
            tree_bucket_next.pop();
        root->operands_list.push_back(tree_operand);

        //Correcting location and time
        CorrectLocationAndTime(tree_operand->tree_location, tree_operand->tree_location_value, tree_operand->tree_time, tree_operand->tree_time_value);
    }
}
/** will not come into picture ever **/
void FirstNotBracketLastBracket(stack<string> &tree_next_bucket, tree_expression *root) {
    while (tree_next_bucket.top() != "(") {
        tree_expression *child = new tree_expression();
        NoOperatorAnywhere(tree_next_bucket, child);
        root->operands_list.push_back(child);
    }
    tree_expression *second_child = new tree_expression();
    stack<string> tree_next_next_bucket;
    string first, first_next;
    string last, second_last;
    TreeInitialSetup(tree_next_bucket, tree_next_next_bucket, first, first_next, last, second_last);
    CreationOfChildren(tree_next_bucket, first, last, second_last, second_child);
    root->operands_list.push_back(second_child);
}
void FirstBracketLastNotBracket(stack<string> &tree_next_bucket, tree_expression *root) {
    stack<string> tree_stack_first;
    stack<string> tree_stack_last;

    int bracket_ct = 0;
    while (!(tree_next_bucket.top() == ")" && bracket_ct == 1)) {
        if (tree_next_bucket.top() == "(")
            bracket_ct++;
        else if (tree_next_bucket.top() == ")")
            bracket_ct--;
        tree_stack_first.push(tree_next_bucket.top());
        tree_next_bucket.pop();
    }
    tree_stack_first.push(tree_next_bucket.top());
    tree_next_bucket.pop();
    while (!tree_next_bucket.empty()) {
        tree_stack_last.push(tree_next_bucket.top());
        tree_next_bucket.pop();
    }

    //Reversing the stacks
    ReverseStack(tree_stack_first);
    ReverseStack(tree_stack_last);

    //First Child
    tree_expression *first_child = new tree_expression();
    stack<string> tree_next_next_bucket;
    string first, first_next;
    string last, second_last;

    TreeInitialSetup(tree_stack_first, tree_next_next_bucket, first, first_next, last, second_last);
    CreationOfChildren(tree_next_next_bucket, first, last, second_last, first_child);
    root->operands_list.push_back(first_child);
    //Second Child
    EntireBucketIsOperands(tree_stack_last, root);
}
void SwappingTwoOperandsInOperandList(tree_expression *&left, tree_expression *&right) {
    tree_expression *temp_exp = left;
    left = right;
    right = temp_exp;
}
void CreationOfChildren(stack<string> &tree_next_bucket, string &first, string &last, string &second_last, tree_expression *root) {
    //Creation of children
    if (last == " " || last == "") {
        NoOperatorAnywhere(tree_next_bucket, root);
    } else {
        //There are operators
        root->IsOperator = true;
        root->tree_op = last;

        //4 cases
        if (first != "(" && second_last != ")") {
            EntireBucketIsOperands(tree_next_bucket, root);
        } else if (first != "(" && second_last == ")") {
            FirstNotBracketLastBracket(tree_next_bucket, root);
        } else if (first == "(" && second_last != ")") {
            FirstBracketLastNotBracket(tree_next_bucket, root);
        } else {
            stack<string> tree_next_next_bucket;
            string n_first, n_first_next;
            string n_last, n_second_last;

            TreeInitialSetup(tree_next_bucket, tree_next_next_bucket, n_first, n_first_next, n_last, n_second_last);
            CreationOfChildren(tree_next_next_bucket, n_first, n_last, n_second_last, root);
        }

        //swapping operands
        string abnormal_flag = Abnormal;
        if (_stricmp(second_last.c_str(), abnormal_flag.c_str()) == 0 || root->tree_op == DivideOperator) {
            SwappingTwoOperandsInOperandList(root->operands_list[0], root->operands_list[1]);
        }
    }
}
void TreeConverter(stack<string> &bucket, tree_expression *root) {
    stack<string> tree_bucket = bucket;
    stack<string> tree_next_bucket;
    string first, first_next;
    string last, second_last;
    TreeInitialSetup(tree_bucket, tree_next_bucket, first, first_next, last, second_last);
    CreationOfChildren(tree_next_bucket, first, last, second_last, root);
}
void CheckTreeFormation(tree_expression *root) {
    cout<<"Is operator: "<<root->IsOperator<<"\n";
    cout<<"Tree Op: "<<root->tree_op<<"\n";
    cout<<"Tree var: "<<root->tree_variable<<"\n";
    cout<<"Tree action: "<<root->tree_action<<"\n";
    cout<<"Tree location: "<<root->tree_location<<"\n";
    cout<<"Tree location value: "<<root->tree_location_value<<"\n";
    cout<<"Tree time: "<<root->tree_time<<"\n";
    cout<<"Tree time value: "<<root->tree_time_value<<"\n";
    for (register int i = 0; i < root->operands_list.size(); i++) {
        CheckTreeFormation(root->operands_list[i]);
    }
}
int main() {
    stack<string> tree_bucket;
    char command[100];
    gets(command);
    vector<string> words;
    stack<string> bucket;
    string nx_command = EmptyString;
    queue<Operand*> ods;
    queue<Operand*> temp;

    string cmd = command;
    cmd = ReplaceCommaBySpaceAnd(cmd);

    istringstream iss(cmd);
    while (getline(iss, cmd, ' '))
        words.push_back(cmd);

    string word;
    Operand *currod = new Operand();
    register int is_op, bracket_ct = 0, first_time = 0, currop_ind = -1;
    int currop[100];
    int opcount[100] = {0};

    for(register int i = 0; i < words.size(); i++) {
        word = words[i];
        //Set NX Command
        SetNXCmd(word, nx_command);
        //Check for operator
        is_op = IsPresentInOperators(word);
        if(is_op != -1) {
            bucket.push(ClosedBracket);bracket_ct++;
            bucket.push(operators[is_op][1]);
            currop[++currop_ind] = IsWhichOperator(operators[is_op][1]);
            //Check for subtraction if between/from interpretation
            CheckForAbnormalOperators(operators[is_op][1], i, words, bucket);
            //Find its operands
            i++;
            while (i < words.size()) {
                word = words[i];
                //Set NX Command
                SetNXCmd(word, nx_command);
                is_op = IsPresentInOperators(word);
                if (is_op != -1) {
                    if (((currod->GetVariable() != DefaultComponent) || (currod->GetAction() != DefaultAction)) && ((opcount[currop_ind]+1) == currop[currop_ind])) {

                    } else if (i != 0) {
                        if ((IsPartOfOperand(words[i-1])) || (IsStringANumber(words[i-1]))) {
                            bucket.push(ClosedBracket);bracket_ct++;
                            FillTheBucket(currod, bucket);
                            ResetCurrentOperand(currod);
                            bucket.push(operators[is_op][1]);
                            currop[++currop_ind] = IsWhichOperator(operators[is_op][1]);
                            opcount[currop_ind] += 1;
                            break;
                        }
                    }
                    Operand *od = new Operand(*currod);
                    ods.push(od);
                    opcount[currop_ind] += 1;
                    bool inside = FixMissingEntriesAndFillTheBucket(ods, bucket);
                    ResetCurrentOperand(currod);
                    if (inside == 1) {
                        if (opcount[currop_ind] == currop[currop_ind]) {
                            bucket.push(OpenBracket);
                            bucket.push(BlankSpace);
                            currop_ind--;bracket_ct--;
                            currop[++currop_ind] = IsWhichOperator(operators[is_op][1]);
                            if (currop[currop_ind] != 1) {
                                //Bracket resolution for this particular condition
                                PushBracketBackWhenOperator(bracket_ct, bucket);
                                //Bracket resolution is done
                                bucket.push(operators[is_op][1]);
                                CheckForAbnormalOperators(operators[is_op][1], i, words, bucket);
                            } else {
                                bucket.push(ClosedBracket);bracket_ct++;
                                bucket.push(operators[is_op][1]);
                                CheckForAbnormalOperators(operators[is_op][1], i, words, bucket);
                            }
                        } else {
                            bucket.push(BlankSpace);
                            i--;
                        }
                    }
                    else {
                            opcount[currop_ind] -= 1;
                            bucket.push(BlankSpace);
                            i--;
                    }
                    break;
                }
                if (IsVariable(word)) {
                    SetVariable(word, currod, ods, bucket, currop, opcount, bracket_ct, currop_ind);
                } else if (IsAction(word)) {
                    SetAction(word, currod, ods, bucket, currop, opcount, bracket_ct, currop_ind);
                } else if (IsLocation(word)) {
                    SetLocation(word, currod, words, i);
                } else if (IsTime(word)) {
                    SetTime(word, currod, words, i);
                }
                i++;
            }
        } else if(IsPartOfOperand(word)) {
            if ((currop_ind == -1) && (first_time == 0)) {
                bucket.push(ClosedBracket);bracket_ct++;first_time = 1;
            }
            //do operand part similar to above block. No need to check is empty or something
            if (IsVariable(word)) {
                SetVariable(word, currod, ods, bucket, currop, opcount, bracket_ct, currop_ind);
            } else if(IsAction(word)) {
                SetAction(word, currod, ods, bucket, currop, opcount, bracket_ct, currop_ind);
            } else if (IsLocation(word)) {
                SetLocation(word, currod, words, i);
            } else if (IsTime(word)) {
                SetTime(word, currod, words, i);
            }
        }
    }
    Operand *od = new Operand(*currod);
    ods.push(od);
    opcount[currop_ind] += 1;
    bool inside = FixMissingEntriesAndFillTheBucket(ods, bucket);
    if (inside == 0)
        opcount[currop_ind] -= 1;
    bucket.push(OpenBracket);bracket_ct--;
    if (bracket_ct > 0) {
        while (bracket_ct != 0) {
            bucket.push(OpenBracket);
            bracket_ct--;
        }
    }

    tree_bucket = bucket;
    /* Suppress Printing To Console */
    cout<<"---------------------------------------------------------------------------"<<endl;
    cout<<"NX Command: "<<nx_command<<endl;
    cout<<"Parsed Statement"<<endl;
    while (bucket.size() > 0) {
        cout<<bucket.top();
        bucket.pop();
    }
    cout<<endl<<"---------------------------------------------------------------------------"<<endl;
    cout<<endl<<endl;

    //Convert stack to tree
    tree_expression *root = new tree_expression();
    TreeConverter(tree_bucket, root);

    //Checking tree formation
    cout<<"Checking tree formation\n\n";
    CheckTreeFormation(root);

    return 0;
}
