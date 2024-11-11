#include <iostream>
#include <string>
//난수를 생성하기 위한 라이브러리1
#include <cstdlib>
//난수를 생성하기 위한 라이브러리2
#include <ctime>

using namespace std;


//질문과 대답 개수
const int questionCount = 5;
const int answerCount = 2;
//관리해야 하는 포인트 4가지 (종교, 자산, 군사, 민심) 범위 : 0~10
int religionPoint = 5, propertyPoint = 5, militaryPoint = 5, citizenPoint = 5;
//index 0에는 질문, 1, 2에는 그에 대한 대답이 담겨있는 2차원 배열 선언
string questionsAndAnswers[questionCount][answerCount + 1] = {
    {"동쪽 국경 부근 산악지대에 무법자가 날뛰고 있습니다.","1. 그냥 내버려두게.", "2. 병사를 파견하게."},
    {"역병 감염이 의심되는 배 한 척이 있습니다. 항구를 잠시 봉쇄하시겠습니까?","1. 아니.", "2. 당장 봉쇄하게."},
    {"큰 규모의 지진이 발생했습니다! 지시를 내려주십시오!","1. 피해자들을 구출하게.", "2. 파괴된 수도부터 수리하게."},
    {"폐하! 올해는 풍년입니다.","1. 병사들에게 식량을 주게.", "2. 백성들에게 음식을 주게."},
    {"폐하, 이웃나라의 젊은 공주와 결혼하시겠습니까?","1. 예", "2. 아니오"}
};
//index 0~3은 1번 선택지에 대한 종교, 자산, 군사, 민심 포인트 변화
//index 4~7은 2번 선택지에 대한 종교, 자산, 군사, 민심 포인트 변화
int pointChange[questionCount][8] = {   
    {0, 1, 0, -1, 0, 1, -1, 2},
    {0, 2, -2, -1, 0, -2, 0, 2},
    {1, -1, -1, 2, -1, 1, 0, -3},
    {0, 1, 2, -1, 2, 0, 0, 3},
    {2, 2, 2, -2, 0, 0, 0, 0}
};
//각 포인트를 시각화해주는 함수
void drawPoint(int point, string str);
//각 포인트와 일 수를 출력해주는 함수
void drawUI(int dayCount);
//포인트가 오르고 내렸을 때의 출력을 위한 함수
void pointUp(int questionNumber, int index, string pointName);
void pointDown(int questionNumber, int index, string pointName);
//각 포인트 변화를 출력해주는 함수
void changePointPrint(int questionNumber, int defaultIndex);
//포인트가 0이 됐는지 확인하는 bool 함수
bool checkZeroPoint();

int main(){
    //시간에 따른 완전한 난수를 생성하기 위한 코드
    srand(static_cast<unsigned int>(time(0)));
    //일차 변수
    int dayCount = 0;

    cout << "당신은 소공왕국의 국왕입니다." << endl <<"종교, 자산, 군사, 민심을 컨트롤 해야하며, 이 중 하나라도 0이 될 시 게임이 종료됩니다." << endl;
    cout << "각 포인트의 최댓값은 10입니다." << endl;

    while(1){
        int answerNumber;
        int answerNumberIndex;
        int defaultIndex;
        dayCount++;
        if (dayCount == 10){
            cout << "10일차가 되었습니다. 당신은 국왕으로서의 책무를 다했습니다. 축하합니다." << endl;
            break;
        }
        //일수와 각 포인트 출력
        drawUI(dayCount);
        //랜덤한 질문 인덱스 생성
        int questionNumber = rand() % questionCount;
        //질문과 답변 출력
        cout << questionsAndAnswers[questionNumber][0] << endl << questionsAndAnswers[questionNumber][1] << endl << questionsAndAnswers[questionNumber][2] << endl;
        //답변 입력받기
        cin >> answerNumber;
        //답변에 따른 인덱스 설정 (답이 1이면 index를 0으로, 2면 index를 4로 설정)
        defaultIndex = (answerNumber - 1) * 4;
        //기본 인덱스 ~ +3까지 각 포인트에 증감. 최댓값은 10.
        religionPoint = min(religionPoint + pointChange[questionNumber][defaultIndex], 10);
        propertyPoint = min(propertyPoint + pointChange[questionNumber][defaultIndex+1], 10);
        militaryPoint = min(militaryPoint + pointChange[questionNumber][defaultIndex+2], 10);
        citizenPoint = min(citizenPoint + pointChange[questionNumber][defaultIndex+3], 10);
        //포인트 변화량 문구 출력
        changePointPrint(questionNumber, defaultIndex);
        //포인트 0인지 확인, 맞다면 문구 출력 후 break
        if (checkZeroPoint() == true) break;
    }
    cout << "게임을 종료합니다." << endl;
}
void drawPoint(int point, string str){
    cout << "| " << str << " - ";
    for (int i = 0; i < point; i++){
        cout << "▮";
    }
    for (int i = 0; i < 10 - point; i++){
        cout << "▯";
    }
    cout << "  |" << endl;
}
void drawUI(int dayCount){
    cout << " ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;
    cout << "|       " << dayCount << "일차 " << "      ";
    if (dayCount < 10)
        cout << " |" <<endl;
    else cout << "|" << endl;
    drawPoint(religionPoint, "종교");
    drawPoint(propertyPoint, "자산");
    drawPoint(militaryPoint, "군사");
    drawPoint(citizenPoint, "민심");
    cout << " ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"<< endl;
}
void changePointPrint(int questionNumber, int defaultIndex){
    string pointName[4] = {"종교", "자산", "군사", "민심"};
    cout << "question Number = " << questionNumber << ", default Index = " << defaultIndex << endl;
    for (int i = 0; i < 4; i++){
        if (pointChange[questionNumber][defaultIndex + i] > 0){
            pointUp(questionNumber, defaultIndex + i, pointName[i]);
        }
        else if (pointChange[questionNumber][defaultIndex + i] < 0){
            pointDown(questionNumber, defaultIndex + i, pointName[i]);
        }
    }
}
void pointUp(int questionNumber, int index, string pointName){
    cout << pointName << "포인트가 " << pointChange[questionNumber][index] <<"만큼 증가했습니다." << endl;
}
void pointDown(int questionNumber, int index, string pointName){
    cout << pointName << "포인트가 " << -1 * pointChange[questionNumber][index] <<"만큼 감소했습니다." << endl;
}
bool checkZeroPoint(){
    if (religionPoint <= 0){
            cout << "종교 포인트가 0이 되었습니다." << endl << "당신은 종교인들의 반란을 막지 못하고 십자가에 걸려 죽음을 맞이했습니다." << endl;
        }
        if (propertyPoint <= 0){
            cout << "자산 포인트가 0이 되었습니다." << endl << "소공왕국의 국고는 결국 바닥이 나서 파산했습니다." << endl;
        }
        if (militaryPoint <= 0){
            cout << "군사 포인트가 0이 되었습니다." << endl << "소공왕국의 병사 수가 급격하게 감소했습니다. 이에 옆 왕국에서 들어온 침략을 막지 못하고 멸망했습니다." << endl;
        }
        if (citizenPoint <= 0){
            cout << "민심 포인트가 0이 되었습니다." << endl << "모든 백성들이 당신에게 단단히 화가 났습니다. 반기를 들고 왕국에 쳐들어와 당신은 죽음을 맞이했습니다." << endl;
        }
}