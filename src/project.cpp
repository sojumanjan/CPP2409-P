#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Question.h"

using namespace std;


//질문과 대답 개수
const int questionCount = 5;
const int answerCount = 2;
//관리해야 하는 포인트 4가지 (종교, 자산, 군사, 민심) 범위 : 0~10
int religionPoint = 5, propertyPoint = 5, militaryPoint = 5, citizenPoint = 5;

//각 포인트를 시각화해주는 함수
void DrawPoint(int point, string str);
//각 포인트와 일 수를 출력해주는 함수
void DrawUI(int dayCount);
//포인트가 오르고 내렸을 때의 출력을 위한 함수
void PointUp(int question_number, int index, string pointName, Question* ques);
void PointDown(int question_number, int index, string pointName, Question* ques);
//각 포인트 변화를 출력해주는 함수
void ChangePointPrint(int question_number, int default_index, Question* ques);
//포인트가 0이 됐는지 확인하는 bool 함수
bool CheckZeroPoint();
string ConditionCheck();

int main(){
    //시간에 따른 완전한 난수를 생성하기 위한 코드
    srand(static_cast<unsigned int>(time(0)));
    Question* question;
    //일차 변수
    int dayCount = 0;

    cout << "당신은 소공왕국의 국왕입니다." << endl <<"종교, 자산, 군사, 민심을 컨트롤 해야하며, 이 중 하나라도 0이 될 시 게임이 종료됩니다." << endl;
    cout << "각 포인트의 최댓값은 10입니다." << endl;

    while(1){
        int answer_number;
        int default_index;
        int question_number;

        string condition = ConditionCheck();

        //포인트 컨디션이 정상일 때
        if (condition == "normal"){
            question = new GeneralQuestion(); //question을 일반 질문 객체로 설정
            question_number = rand() % question->GetQuestions().size(); //랜덤 질문 선정
        }
        //1/4 확률로 특수 질문 객체로 설정
        else{
            int random_int = rand() % 1;
            if (random_int == 0) {
                question = new SpecialQuestion(condition);
            }
            else {
                question = new GeneralQuestion();
            }
            question_number = rand() % question->GetQuestions().size();
        }

        //일 수 추가 후 종료조건 확인
        dayCount++;
        if (dayCount == 10){
            cout << "10일차가 되었습니다. 당신은 국왕으로서의 책무를 다했습니다. 축하합니다." << endl;
            break;
        }

        //일수와 각 포인트 출력
        DrawUI(dayCount);

        //질문과 답변 출력
        for (int i = 0; i < 3; i++){
            cout << question->GetQuestions()[question_number][i] << endl;
        }

        //답변 입력받기
        cin >> answer_number;
        //답변에 따른 인덱스 설정 (답이 1이면 index를 0으로, 2면 index를 4로 설정)
        default_index = (answer_number - 1) * 4;
        //기본 인덱스 ~ +3까지 각 포인트에 증감. 최댓값은 10.
        religionPoint = min(religionPoint + question->GetPointChange(question_number, default_index), 10);
        propertyPoint = min(propertyPoint + question->GetPointChange(question_number, default_index+1), 10);
        militaryPoint = min(militaryPoint + question->GetPointChange(question_number, default_index+2), 10);
        citizenPoint = min(citizenPoint + question->GetPointChange(question_number, default_index+3), 10);
        //답변에 대한 문구 출력
        cout << question->GetQuestions()[question_number][answer_number + 2] << endl;
        //포인트 변화량 문구 출력
        ChangePointPrint(question_number, default_index, question);
        //포인트 0인지 확인, 맞다면 문구 출력 후 break
        if (CheckZeroPoint() == true) break;
    }
    cout << "게임을 종료합니다." << endl;
}
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
void DrawUI(int dayCount){
    cout << " ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;
    cout << "|       " << dayCount << "일차 " << "      ";
    if (dayCount < 10)
        cout << " |" <<endl;
    else cout << "|" << endl;
    DrawPoint(religionPoint, "종교");
    DrawPoint(propertyPoint, "자산");
    DrawPoint(militaryPoint, "군사");
    DrawPoint(citizenPoint, "민심");
    cout << " ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"<< endl;
}
void ChangePointPrint(int question_number, int defaultIndex, Question* ques){
    string pointName[4] = {"종교", "자산", "군사", "민심"};
    for (int i = 0; i < 4; i++){
        if (ques->GetPointChange(question_number, defaultIndex + i) > 0){
            PointUp(question_number, defaultIndex + i, pointName[i], ques);
        }
        else if (ques->GetPointChange(question_number, defaultIndex + i)){
            PointDown(question_number, defaultIndex + i, pointName[i], ques);
        }
    }
}
void PointUp(int question_number, int index, string pointName, Question* ques){
    cout << pointName << "포인트가 " << ques->GetPointChange(question_number, index) <<"만큼 증가했습니다." << endl;
}
void PointDown(int question_number, int index, string pointName, Question* ques){
    cout << pointName << "포인트가 " << -1 * ques->GetPointChange(question_number, index) <<"만큼 감소했습니다." << endl;
}
bool CheckZeroPoint(){
    if (religionPoint <= 0){
        cout << "종교 포인트가 0이 되었습니다." << endl << "당신은 종교인들의 반란을 막지 못하고 십자가에 걸려 죽음을 맞이했습니다." << endl;
        return true;
    }
    if (propertyPoint <= 0){
        cout << "자산 포인트가 0이 되었습니다." << endl << "소공왕국의 국고는 결국 바닥이 나서 파산했습니다." << endl;
        return true;
    }
    if (militaryPoint <= 0){
        cout << "군사 포인트가 0이 되었습니다." << endl << "소공왕국의 병사 수가 급격하게 감소했습니다. 이에 옆 왕국에서 들어온 침략을 막지 못하고 멸망했습니다." << endl;
        return true;
    }
    if (citizenPoint <= 0){
        cout << "민심 포인트가 0이 되었습니다." << endl << "모든 백성들이 당신에게 단단히 화가 났습니다. 반기를 들고 왕국에 쳐들어와 당신은 죽음을 맞이했습니다." << endl;
        return true;
    }
    return false;
}

string ConditionCheck(){
    if (religionPoint >= 9) return "r+";
    else if (propertyPoint >= 9) return "p+";
    else if (militaryPoint >= 9) return "m+";
    else if (citizenPoint >= 9) return "c+";
    else if (religionPoint <= 2) return "r-";
    else if (propertyPoint <= 2) return "p-";
    else if (militaryPoint <= 2) return "m-";
    else if (citizenPoint <= 2) return "c-";
    else return "normal";
}