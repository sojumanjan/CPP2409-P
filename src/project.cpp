#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Question.h"

using namespace std;


//대답 개수 (최대 3)
const int answer_count = 2;
const int end_day = 15;
//관리해야 하는 포인트 4가지 (종교, 자산, 군사, 민심) 범위 : 0~10
int religion_point = 5, property_point = 5, military_point = 5, citizen_point = 5;

//각 포인트를 시각화해주는 함수
void DrawPoint(int point, string str);
//각 포인트와 일 수를 출력해주는 함수
void DrawUI(int day_count);
//포인트가 오르고 내렸을 때의 출력을 위한 함수
void PointUp(int question_number, int index, string point_name, Question* ques);
void PointDown(int question_number, int index, string point_name, Question* ques);
//각 포인트 변화를 출력해주는 함수
void ChangePointPrint(int question_number, int default_index, Question* ques);
//포인트가 0이 됐는지 확인하는 bool 함수
bool CheckZeroPoint();
string ConditionCheck();

int main(){
    //시간에 따른 완전한 난수를 생성하기 위한 코드
    srand(static_cast<unsigned int>(time(0)));
    Question* question;
    GeneralQuestion general_question;
    SpecialQuestion special_question;
    //일차 변수
    int day_count = 0;

    cout << "당신은 소공왕국의 국왕입니다." << endl <<"종교, 자산, 군사, 민심을 컨트롤 해야하며, 이 중 하나라도 0이 될 시 게임이 종료됩니다." << endl;
    cout << "각 포인트의 최댓값은 10입니다." << endl;

    while(1){
        int answer_number;
        int default_index;
        int question_number;

        string condition = ConditionCheck();
            //포인트 컨디션이 정상일 때
            if (condition == "normal"){
                if (!general_question.GetQuestions(condition).empty()){
                    question = &general_question; //question을 일반 질문 객체로 설정
                }
                else{
                    cout << "남아있는 일반 질문이 없습니다. 게임을 종료합니다." << endl;
                    break;
                }
            }
            //포인트가 0일 때 특수 질문 하도록 설정
            else if (CheckZeroPoint() == true){
                question = &special_question;
            }
            //1/4 확률로 특수 질문 객체로 설정
            else {
                if (!special_question.GetQuestions(condition).empty()){
                    int random_int = rand() % 4;
                    if (random_int == 0) {
                        question = &special_question;
                    }
                    else {
                        question = &general_question;
                    }
                }
                else{
                    cout << "남아있는 특수 질문이 없습니다." << endl;
                    question = &general_question;
                }
            }
            //랜덤 질문 인덱스 생성
            question_number = rand() % question->GetQuestions(condition).size();

        //일 수 추가 후 종료조건 확인
        day_count++;
        if (day_count == end_day){
            cout << end_day << "일차가 되었습니다. 당신은 국왕으로서의 책무를 다했습니다. 축하합니다." << endl;
            break;
        }

        //일수와 각 포인트 출력
        DrawUI(day_count);

        //질문과 답변 출력
        for (int i = 0; i < answer_count + 1; i++){
            cout << question->GetQuestions(condition)[question_number][i] << endl;
        }

        //답변 입력받기
        while(1){
            cin >> answer_number;
            if (answer_number == 1 || answer_number == 2)
            break;
            else{
                cout << "잘못된 선택입니다. 다시 선택하세요. " << endl;
                continue;
            }
        }
        
        //답변에 대한 문구 출력
        cout << question->GetQuestions(condition)[question_number][answer_number + 2] << endl;
        //포인트가 0이라면 특수 질문 바로 위에서 출력 후 break.
        if (CheckZeroPoint() == true) break;
        //답변에 따른 인덱스 설정 (답이 1이면 index를 0으로, 2면 index를 4로 설정)
        default_index = (answer_number - 1) * 4;
        //기본 인덱스 ~ +3까지 각 포인트에 증감. 최댓값은 10, 최솟값은 0
        religion_point = max(min(religion_point + question->GetPointChange(question_number, default_index), 10), 0);
        property_point = max(min(property_point + question->GetPointChange(question_number, default_index+1), 10), 0);
        military_point = max(min(military_point + question->GetPointChange(question_number, default_index+2), 10), 0);
        citizen_point = max(min(citizen_point + question->GetPointChange(question_number, default_index+3), 10), 0);
        
        //포인트 변화량 문구 출력
        ChangePointPrint(question_number, default_index, question);
        //사용했던 질문 삭제
        question->DeleteQuestion(question_number);
    }
    cout << "최종 스코어입니다." << endl;
    DrawUI(day_count);
    cout << "게임을 종료합니다." << endl;
}

// ------------ 함수 구현 ------------- //
void DrawPoint(int point, string str){
    cout << "| " << str << " - ";
    for (int i = 0; i < point; i++){
        cout << "▮";
    }
    for (int i = 0; i < 10 - point; i++){
        cout << "▯";
    }
    cout << "  |" << endl;
}
void DrawUI(int day_count){
    cout << " ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;
    cout << "|       " << day_count << "일차 " << "      ";
    if (day_count < 10)
        cout << " |" <<endl;
    else cout << "|" << endl;
    DrawPoint(religion_point, "종교");
    DrawPoint(property_point, "자산");
    DrawPoint(military_point, "군사");
    DrawPoint(citizen_point, "민심");
    cout << " ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"<< endl;
}
void ChangePointPrint(int question_number, int default_index, Question* ques){
    string point_name[4] = {"종교", "자산", "군사", "민심"};
    for (int i = 0; i < 4; i++){
        if (ques->GetPointChange(question_number, default_index + i) > 0){
            PointUp(question_number, default_index + i, point_name[i], ques);
        }
        else if (ques->GetPointChange(question_number, default_index + i)){
            PointDown(question_number, default_index + i, point_name[i], ques);
        }
    }
}
void PointUp(int question_number, int index, string point_name, Question* ques){
    cout << point_name << "포인트가 " << ques->GetPointChange(question_number, index) <<"만큼 증가했습니다." << endl;
}
void PointDown(int question_number, int index, string point_name, Question* ques){
    cout << point_name << "포인트가 " << -1 * ques->GetPointChange(question_number, index) <<"만큼 감소했습니다." << endl;
}
bool CheckZeroPoint(){
    if (religion_point <= 0 || property_point <= 0 || military_point <= 0 || citizen_point <= 0){
        return true;
    }
    return false;
}

string ConditionCheck(){
    vector<string> condition;
    //특정 포인트가 0이 되었을 때 컨디션
    if (religion_point <= 0) return "r--";
    if (property_point <= 0) return "p--";
    if (military_point <= 0) return "m--";
    if (citizen_point <= 0) return "c--";
    //특정 포인트가 2이하, 9이상일 때의 컨디션
    if (religion_point >= 9) condition.push_back("r+");
    if (property_point >= 9) condition.push_back("p+");
    if (military_point >= 9) condition.push_back("m+");
    if (citizen_point >= 9) condition.push_back("c+");
    if (religion_point <= 2) condition.push_back("r-");
    if (property_point <= 2) condition.push_back("p-");
    if (military_point <= 2) condition.push_back("m-");
    if (citizen_point <= 2) condition.push_back("c-");
    //특수 상태라면,(중복가능) 랜덤하게 리턴.
    if (condition.empty() == false){
        return condition[rand() % condition.size()];
    }
    else return "normal";
}