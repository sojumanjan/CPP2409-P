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
    vector<vector<string>> InitializeQuestions(string file){
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
};

class GeneralQuestion : public Question{
public:
    GeneralQuestion(){
        questions = InitializeQuestions("questions.txt");
        //index 0 = 질문, 1, 2 = 질문에 대한 선택지, 3, 4 = 선택지에 대한 문구
        point_change = {   
            {0, -1, 0, -1, 0, 1, -1, 2},
            {0, 2, -1, -1, 0, -2, 0, 2},
            {1, -1, -1, 2, -1, 1, 0, -2},
            {0, 1, 2, -1, 0, 0, 0, 2},
            {2, 2, 2, -2, 0, 0, 0, 0}
        };
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
        point_change_list = {
            {10, -3, 0, -2, -5, 0, 0, 0},
            {3, 0, 0, -3, -10, 0, 0, 0},
            {2, -3, 2, 2, 0, 0, 0, 0},
            {3, 3, 3, -1, 0, 0, 0, 1},
            {0, 3, -3, 3, 0, 0, -1, -1},
            {-2, 0, 3, -5, 0, 0, 0, 2},
            {-2, 0, 0, -2, 2, 0, 0, 2},
            {-2, 0, -2, 2, 0, -2, 0, 2}
        };
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