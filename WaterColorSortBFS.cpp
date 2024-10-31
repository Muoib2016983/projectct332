#include<stdio.h>
#include<stdlib.h>
#include<queue>
#include<stack>
#define Rows 4 // 4 muc nuoc
#define Cols 5 // 5 ong nuoc
#define Maxlength 1000
// khai bao cau truc trang thai
typedef struct {
	int M[Rows][Cols];
} State;
using namespace std;
const char *action[] = {"First State",
                        "Pour color tubes 1 to tubes 2",
                        "Pour color tubes 1 to tubes 3",
                        "Pour color tubes 1 to tubes 4",
                        "Pour color tubes 1 to tubes 5",
                        "Pour color tubes 2 to tubes 1",
                        "Pour color tubes 2 to tubes 3",
                        "Pour color tubes 2 to tubes 4",
                        "Pour color tubes 2 to tubes 5",
                        "Pour color tubes 3 to tubes 1",
                        "Pour color tubes 3 to tubes 2",
                        "Pour color tubes 3 to tubes 4",
                        "Pour color tubes 3 to tubes 5",
                        "Pour color tubes 4 to tubes 1",
                        "Pour color tubes 4 to tubes 2",
                        "Pour color tubes 4 to tubes 3",
                        "Pour color tubes 4 to tubes 5",
                        "Pour color tubes 5 to tubes 1",
                        "Pour color tubes 5 to tubes 2",
                        "Pour color tubes 5 to tubes 3",
                        "Pour color tubes 5 to tubes 4",
                       };
// Khoi tao trang thai voi gia tri la 0
void makeNull_State(State* state) {
	int i,j;
	for(i=0; i<Rows; i++)
		for(j=0; j<Cols; j++)
			state->M[i][j]=0;
}

//Ham in trang thai
void print_State(State state) {
	int i,j;
	for(i=Rows-1; i>=0; i--) {
		for(j=0;j<=Cols-1;j++)
			printf("|%d| ",state.M[i][j]);
		printf("\n");
	}
	for(i=0; i<Cols; i++){
		printf("%c%c%c ",212,205,190);
	}
	printf("\n");
}
// Ham so sanh 2 trang thai
int compare_States(State s1, State s2) {
	int i,j;
	for (i = 0; i < Rows; i++) {
		for (j = 0; j < Cols; j++) {
			if (s1.M[i][j] != s2.M[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}

int goalcheck(State state, State GOAL) {
	return compare_States(state, GOAL);
}

int Pour_color(State state, State *result, int x, int y) {
    *result = state;
    int i, j;
    if (state.M[0][x] != 0 && state.M[3][y] == 0) { //Kiem tra phan tu dau cuoi cung cua ong nguon & kiem tra phan tu dau tien cua ong dich
    												// Chu yeu de giam do phuc tap cua bai toan (truong hop ong empty chuyen den ong full)
        for (i = Rows - 1; i >= 0; i--) {			// Cho vong lap chay tu tren xuong i = 3 chay toi i= 0
            if (state.M[i][x] != 0) {				
                for (j = 0; j < Rows; j++) {			// Cho vong lap chay tu duoi len j = 0 toi j = 3
                    if (state.M[j][y] == 0){			
						if (j == 0 || (j > 0 && state.M[i][x] == state.M[j-1][y])){	// j = 0 thi ong rong, khong can kiem tra mau o duoi
																					// j>0, kiem tra mau o duoi phai cung mau voi mau chuyen vao
	                        result->M[j][y] = state.M[i][x];						//vi tri nhan mau
	                        result->M[i][x] = 0;									//vi tri chuyen mau di
	                        return 1;
	                    }
                    }
                }
                break;
            }
        }
    } else {
        return 0;
    }
}
// Khai bao Node
typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;

//Kiem tra mot trang thai dau vao no co trong hang doi hay chua, co roi thi xoa no ra khoi hang doi
int find_State(State state, queue<Node *> openQueue){
	while(!openQueue.empty()){
		if(compare_States(openQueue.front()->state,state))
			return 1;
		openQueue.pop();	
	}
	return 0;
}
//BFS - (Trang thai ban dau - Trang thai ket thuc)
Node* BFS_Algorithm(State state, State goal){
	// Khoi tao 2 hang doi Open va Close
	queue<Node *> Open_BFS;
	queue<Node *> Close_BFS;
	//Khai tao nut goc de bieu dien trang thai
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent=NULL;
	root->no_function=0;
	//Tham vao hang doi Open
 	Open_BFS.push(root);
 	//Lay nut dau tien ra khoi Open va dua no vao hang doi Close
	// kiem tra trang thai do co phai goal khong, phai thi thuat toan ket thuc tra ve trang thai hien tai
	while(!Open_BFS.empty()){ 	// Kiem tra hang doi Open co trong khong, trong thi tra ve NULL
		Node* node = Open_BFS.front();
		Open_BFS.pop();
		Close_BFS.push(node);
		if(goalcheck(node->state,goal))
			return node;
		// Bien danh so cac hanh dong thuc hien
		int cnt = 0;
		// Vong lap lap qua tat ca cac hang va cot 
		for(int i = 0; i < Rows; ++i){
			for (int j = 0; j < Cols; ++j){
				if (i != j){		//Bo cac truong hop i va j cung 1 gia tri vi no se la cung mot o, khong the thuc hien hanh dong chuyen mau
					++cnt;
					//Tao trang thai moi
					State newstate;
					//Khoi tao trang thai
					makeNull_State(&newstate);
					//Thuc hien hanh dong do mau
					if(Pour_color(node->state,&newstate, i, j)){
						//Trang thai moi nay phai chua duoc duyet qua trong Open va Close
						if(find_State(newstate,Close_BFS) || find_State(newstate,Open_BFS))
							continue;
						//Node moi duoc tao ra va them vao hang doi Open
						Node* newNode = (Node*)malloc(sizeof(Node));
						newNode->state=newstate;
						newNode->Parent = node;
						newNode->no_function= cnt;
						Open_BFS.push(newNode);	
					}
				}
			}
		}	
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	//Khoi tao cac ham de luu tru
	queue<Node *> queuePrint;
	stack<Node *> Swapstack;
	//Cac Node se duoc luu tru tu trang thai hien tai den trang thai bat dau
	while(node->Parent!=NULL){
		queuePrint.push(node);
		node=node->Parent;
	}
	//Them Node bat dau vao hang doi
	queuePrint.push(node);
	//Bien dem so hanh dong
	int no_action = 0;
	//Dua vao hang Swapstack de luu tru
	while(!queuePrint.empty()){
		Swapstack.push(queuePrint.front());
		queuePrint.pop();
	}
	//moi nut Node duoc lay ra tu Stack in ra hanh dong da thuc hien
	while(!Swapstack.empty()){
		printf("\nAction %d: %s\n",no_action,action[Swapstack.top()->no_function]);
		print_State(Swapstack.top()->state);
		Swapstack.pop();
		//Tang gia tri action len 1
		no_action++;
	}
}
int main(){
	State startState;

//	Tubes1

	startState.M[3][0]=1;
	startState.M[2][0]=2;
	startState.M[1][0]=3;
	startState.M[0][0]=1;

//	Tubes2

	startState.M[3][1]=3;
	startState.M[2][1]=1;
	startState.M[1][1]=2;
	startState.M[0][1]=2;

//	Tubes3
	startState.M[3][2]=3;
	startState.M[2][2]=1;
	startState.M[1][2]=2;
	startState.M[0][2]=3;
//	Tubes4

	startState.M[3][3]=0;
	startState.M[2][3]=0;
	startState.M[1][3]=0;
	startState.M[0][3]=0;

//	Tubes5
	startState.M[3][4]=0;
	startState.M[2][4]=0;
	startState.M[1][4]=0;
	startState.M[0][4]=0;

	State goal;
//	Tubes1

	goal.M[3][0]=0;
	goal.M[2][0]=0;
	goal.M[1][0]=0;
	goal.M[0][0]=0;

//	Tubes2
	goal.M[3][1]=0;
	goal.M[2][1]=0;
	goal.M[1][1]=0;
	goal.M[0][1]=0;

//	Tubes3
	goal.M[3][2]=3;
	goal.M[2][2]=3;
	goal.M[1][2]=3;
	goal.M[0][2]=3;

//	Tubes4

	goal.M[3][3]=2;
	goal.M[2][3]=2;
	goal.M[1][3]=2;
	goal.M[0][3]=2;

//	Tubes5
	goal.M[3][4]=1;
	goal.M[2][4]=1;
	goal.M[1][4]=1;
	goal.M[0][4]=1;
	
////	Tubes1
//
//	startState.M[3][0]=1;
//	startState.M[2][0]=2;
//	startState.M[1][0]=3;
//	startState.M[0][0]=1;
//
////	Tubes2
//
//	startState.M[3][1]=1;
//	startState.M[2][1]=2;
//	startState.M[1][1]=3;
//	startState.M[0][1]=3;
//
////	Tubes3
//	startState.M[3][2]=2;
//	startState.M[2][2]=2;
//	startState.M[1][2]=1;
//	startState.M[0][2]=3;
////	Tubes4
//
//	startState.M[3][3]=0;
//	startState.M[2][3]=0;
//	startState.M[1][3]=0;
//	startState.M[0][3]=0;
//
////	Tubes5
//	startState.M[3][4]=0;
//	startState.M[2][4]=0;
//	startState.M[1][4]=0;
//	startState.M[0][4]=0;
//
//	State goal;
////	Tubes1
//
//	goal.M[3][0]=0;
//	goal.M[2][0]=0;
//	goal.M[1][0]=0;
//	goal.M[0][0]=0;
//
////	Tubes2
//	goal.M[3][1]=3;
//	goal.M[2][1]=3;
//	goal.M[1][1]=3;
//	goal.M[0][1]=3;
//
////	Tubes3
//	goal.M[3][2]=0;
//	goal.M[2][2]=0;
//	goal.M[1][2]=0;
//	goal.M[0][2]=0;
//
////	Tubes4
//
//	goal.M[3][3]=1;
//	goal.M[2][3]=1;
//	goal.M[1][3]=1;
//	goal.M[0][3]=1;
//
////	Tubes5
//	goal.M[3][4]=2;
//	goal.M[2][4]=2;
//	goal.M[1][4]=2;
//	goal.M[0][4]=2;
//	
	
	
	printf("Trang thai ban dau: \n");
	print_State(startState);
	printf("Trang thai ket thuc: \n");
	print_State(goal);
	Node* p=BFS_Algorithm(startState,goal);
	print_WaysToGetGoal(p);
	return 0;
}
