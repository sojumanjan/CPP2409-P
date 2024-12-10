#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Question{
protected:
    vector<vector<string>> questions;
    vector<vector<int>> point_change;
public:
    Question(){}
    //questions 게터
    virtual vector<vector<string>> GetQuestions(string con) = 0;
    //point_change 게터
    virtual int GetPointChange(int question_number, int index) = 0;
    //텍스트파일에서 질문 불러오는 함수
    virtual void DeleteQuestion(int question_number) = 0;
    vector<vector<string>> InitializeQuestions(const string file){
        ifstream ques(file);
        vector<vector<string>> questions;
        string line;
        while (getline(ques, line)) { // 파일에서 한 줄씩 읽기
            vector<string> row; // 한 줄에 해당하는 벡터
            string value;

            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == '|') { // 구분자 만나면 새로운 값 저장
                    row.push_back(value); 
                    value.clear(); // 다음 값을 위해 초기화
                } else {
                    value += line[i]; // 구분자 전까지 값 추가
                }
            }
            if (!value.empty()) { // 마지막 값 처리
                row.push_back(value);
            }
            questions.push_back(row); // 한 줄을 2차원 벡터에 추가
        }
        return questions;
    }
    vector<vector<int>> InitializePointChange(const string file) {
        vector<vector<int>> data;
        ifstream point(file);
        string line;
        // 파일에서 한 줄씩 읽기
        while (getline(point, line)) {
            vector<int> row;  // 한 줄의 데이터를 저장할 벡터
            string value = ""; // 값 저장용 문자열
            // 문자열 순회하며 '|' 처리
            for (char c : line) {
                if (c == '|') {  // 구분자(|)를 만난 경우
                    row.push_back(stoi(value));  // 문자열을 정수로 변환 후 저장
                    value = "";  // value 초기화
                } else {
                    value += c;  // 숫자 문자를 value에 추가
                }
            }
            if (!value.empty()) {
                row.push_back(stoi(value));
            }
            data.push_back(row);  // 완성된 행을 2차원 벡터에 추가
        }
        point.close();  // 파일 닫기
        return data;   // 2차원 벡터 반환
    }
};

class GeneralQuestion : public Question{
public:
    GeneralQuestion(){
        questions = InitializeQuestions("questions.txt");
        //index 0 1 2 3은 각각 종교, 재산, 군사, 민심 포인트의 증감량
        point_change = InitializePointChange("point_change.txt");
    }

    vector<vector<string>> GetQuestions(string con){
        return questions;
    }

    int GetPointChange(int question_number, int index){
        return point_change[question_number][index];
    }
    
    void DeleteQuestion(int question_number){
        if (!questions.empty()){
            questions.erase(questions.begin() + question_number);
            point_change.erase(point_change.begin() + question_number);
        }
    }
};

class SpecialQuestion : public Question{
private:
    vector<vector<string>> questions_list;
    vector<vector<int>> point_change_list;
public:
    SpecialQuestion(){
        //현재 컨디션 (포인트 상태)에 따라 해당하는 상태를 나타내는 "r+, r-, m+ ...." 등의 문자열을 index 5에 추가.
        questions = InitializeQuestions("special_questions.txt");
        point_change_list = InitializePointChange("special_point_change.txt");
    }

    vector<vector<string>> GetQuestions (string con){
        questions_list.clear();
        point_change.clear();
        for (int i = 0; i < questions.size(); i++){
            if (questions[i][5] == con) {
                questions_list.push_back(questions[i]);
                point_change.push_back(point_change_list[i]);
            }
        }
        return questions_list;
    }

    int GetPointChange(int question_number, int index){
        return point_change[question_number][index];
    }

    void DeleteQuestion(int question_number){
        for (int i = 0; i < questions.size(); i++){
            if (questions[i] == questions_list[question_number]){
                questions.erase(questions.begin() + i);
                point_change.erase(point_change.begin() + i);
            }
        }
    }
};