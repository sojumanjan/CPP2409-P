#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Question.h"

using namespace std;


//대답 개수 (최대 3)
const int answer_count = 2;
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
                    question_number = rand() % question->GetQuestions(condition).size(); //랜덤 질문 선정
                }
                else{
                    cout << "남아있는 일반 질문이 없습니다." << endl;
                    break;
                }
            }
            //1/4 확률로 특수 질문 객체로 설정
            else{
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
                question_number = rand() % question->GetQuestions(condition).size();
            }
        

        //일 수 추가 후 종료조건 확인
        day_count++;
        if (day_count == 20){
            cout << "10일차가 되었습니다. 당신은 국왕으로서의 책무를 다했습니다. 축하합니다." << endl;
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
        //답변에 따른 인덱스 설정 (답이 1이면 index를 0으로, 2면 index를 4로 설정)
        default_index = (answer_number - 1) * 4;
        //기본 인덱스 ~ +3까지 각 포인트에 증감. 최댓값은 10.
        religion_point = min(religion_point + question->GetPointChange(question_number, default_index), 10);
        property_point = min(property_point + question->GetPointChange(question_number, default_index+1), 10);
        military_point = min(military_point + question->GetPointChange(question_number, default_index+2), 10);
        citizen_point = min(citizen_point + question->GetPointChange(question_number, default_index+3), 10);
        //답변에 대한 문구 출력
        cout << question->GetQuestions(condition)[question_number][answer_number + 2] << endl;
        //포인트 변화량 문구 출력
        ChangePointPrint(question_number, default_index, question);
        //포인트 0인지 확인, 맞다면 문구 출력 후 break
        if (CheckZeroPoint() == true) break;
        
        question->DeleteQuestion(question_number);
    }
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
    if (religion_point <= 0){
        cout << "종교 포인트가 0이 되었습니다." << endl << "당신은 종교인들의 반란을 막지 못하고 십자가에 걸려 죽음을 맞이했습니다." << endl;
        return true;
    }
    if (property_point <= 0){
        cout << "자산 포인트가 0이 되었습니다." << endl << "소공왕국의 국고는 결국 바닥이 나서 파산했습니다." << endl;
        return true;
    }
    if (military_point <= 0){
        cout << "군사 포인트가 0이 되었습니다." << endl << "소공왕국의 병사 수가 급격하게 감소했습니다. 이에 옆 왕국에서 들어온 침략을 막지 못하고 멸망했습니다." << endl;
        return true;
    }
    if (citizen_point <= 0){
        cout << "민심 포인트가 0이 되었습니다." << endl << "모든 백성들이 당신에게 단단히 화가 났습니다. 반기를 들고 왕국에 쳐들어와 당신은 죽음을 맞이했습니다." << endl;
        return true;
    }
    return false;
}

string ConditionCheck(){
    if (religion_point >= 9) return "r+";
    else if (property_point >= 9) return "p+";
    else if (military_point >= 9) return "m+";
    else if (citizen_point >= 9) return "c+";
    else if (religion_point <= 2) return "r-";
    else if (property_point <= 2) return "p-";
    else if (military_point <= 2) return "m-";
    else if (citizen_point <= 2) return "c-";
    else return "normal";
}