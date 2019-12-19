#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//트리와 배열의 구성노드
typedef struct Node* NodePtr;
typedef struct Node{
unsigned char c;
int n, cnt;
NodePtr l, r;
}Node;

//바이트 코드 생성에 필요한 트리를 만들 때 사용
typedef struct Bit* BitPtr;
typedef struct Bit {
	bool n;
	BitPtr link, blink;  //link: 앞으로 연결, blink: 뒤로 연결
}Bit;

//파일 정보 읽어들임
//문자마다 개수도 같이 조사해서 배열에 저장
void readFile(char*filename,NodePtr arr) {
	FILE*fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("File not found");
		exit(1);
	}
	char k;
	for (int i = 0; i < 128; i++) {
		arr[i].c = (char)i;
		arr[i].n = 0;
		arr[i].l = NULL;
		arr[i].r = NULL;
	}
	while (true) {
		k=fgetc(fp);
		if (k == EOF) break;
		int tmp = k;
		arr[tmp].n += 1;
	}
	fclose(fp);
}
//min heap으로 출현 횟수가 적은 순서에 맞게 노드를 삽입한다.
void insertHeap(NodePtr heapArr, Node heapNode, int num) {
	int temp = heapNode.n;
	num += 1;
	int p = num / 2, c = num;
	while (c >= 2) {
		if (heapArr[p].n > temp) heapArr[c] = heapArr[p];
		else break;
		p /= 2;
		c /= 2;
	}
	heapArr[c].n = heapNode.n;
	heapArr[c].c = heapNode.c;
	heapArr[c].l = heapNode.l;
	heapArr[c].r = heapNode.r;
	return;
}
//파일에 한 번이라도 나타났다면 heap에 넣어준다.
//heapArr[1]은 종결문자에 대한 정보(횟수가 0이므로 첫번째를 유지한다)
int makeHeap(NodePtr readArr,NodePtr heapArr) {
	int i, size = 1;
	heapArr[1].n = 0;
	heapArr[1].c = (char)0x80;
	heapArr[1].l = heapArr[1].r = NULL;
	for (i = 0; i < 128; i++) {
		if (readArr[i].n != 0) {
			insertHeap(heapArr, readArr[i], size);
			size++;
		}
	}
	return size;
}
//heap에서 출현횟수가 가장 작은 노드를 반환하고 재정렬해준다.
//n은 heap의 size
Node deleteHeap(NodePtr heapArr, int n) {
	Node forRet = heapArr[1], temp = heapArr[n];
	int p = 1, c = 2;
	while (c <= n) {
		if ((c < n) && (heapArr[c].n >= heapArr[c + 1].n)) c++;
		if (temp.n <= heapArr[c].n) break;
		else heapArr[p] = heapArr[c];
		c *= 2; 
		p = c / 2;
	}
	heapArr[p] = temp;
	return forRet;
}
//바이트 코드 생성에 필요한 트리를 만들어준다.
//min heap에서 크기가 작은 것부터 불러와 개별 트리를 만들어준 뒤 heap에 다시 넣는다.
//heap의 size-1만큼 반복한다.
NodePtr makeTree(NodePtr heapArr, int size) {
	int i, n = size;
	NodePtr u, v, w = NULL;
	Node tempU, tempV;
	for (i = 1; i < size; i++) {
		tempU = deleteHeap(heapArr, n--);
		tempV = deleteHeap(heapArr, n--);
		u = (NodePtr)malloc(sizeof(Node));
		u->c = tempU.c;
		u->n = tempU.n;
		u->l = tempU.l;
		u->r = tempU.r;
		v = (NodePtr)malloc(sizeof(Node));
		v->c = tempV.c;
		v->n = tempV.n;
		v->l = tempV.l;
		v->r = tempV.r;
		w = (NodePtr)malloc(sizeof(Node));
		w->n = u->n + v->n;
		w->l = u;
		w->r = v;
		if(n!=0) insertHeap(heapArr, *w, n++);
	}
	return w;
}
//트리에 있는 비트 정보를 10진수로 배열에 저장
//010같이 값만 알아서는 안 되는 경우 존재해서 길이도 저장
void getBit(NodePtr bitArr, NodePtr r, BitPtr s, BitPtr w, BitPtr l, int n) {
	/*
	s,w,l은 tree를 이동함에 따라 추가되는 비트를 저장함 ex)root로부터 left right left라면 010을 저장해야하고 s=0,l=0을 저장하고있음
	비트의 자리수와 값을 bitArr에 저장함 010인 경우 값은 2지만 앞의 0도 저장해야하기 때문에 자리수 3을 저장하고 나중에 처리함
	n은 s~l의 개수로 비트에 대한 값을 저장할 때 제곱 정도에 대한 정보. ex)101비트를 저장할 때 n은 3이고 처음 1부터 2의 n-1승을 곱해줘야함.
	*/
	if (r->l != NULL) {
		//전체 트리의 root노드에서 왼쪽으로 갈 때
		if (s == NULL) {
			s = (BitPtr)malloc(sizeof(Bit));
			s->n = false;
			s->link = NULL;
			s->blink = NULL;
			l = s;
			w = s;
		}
		//0bit에 대한 정보를 저장하고 왼쪽으로 이동
		else {
			l = (BitPtr)malloc(sizeof(Bit));
			l->n = false;
			l->link = NULL;
			l->blink = w;
			w->link = l;
			w = l;
		}
		getBit(bitArr, r->l, s, w, l, ++n);
		//사용한 0bit에 대한 정보 삭제
		w = l->blink;
		if(w) w->link = NULL;
		else s = NULL;
		free(l);
		n--;
	}
	if (r->r != NULL) {
		//전체 트리의 root노드에서 오른쪽으로 갈 때
		if (s == NULL) {
			s = (BitPtr)malloc(sizeof(Bit));
			s->n = true;
			s->link = NULL;
			s->blink = NULL;
			l = s;
			w = s;
		}
		//1bit에 대한 정보 저장하고 오른쪽으로 이동
		else {
			l = (BitPtr)malloc(sizeof(Bit));
			l->n = true;
			l->link = NULL;
			l->blink = w;
			w->link = l;
			w = l;
		}
		getBit(bitArr, r->r, s, w, l, ++n);
		//사용한 1bit에 대한 정보 삭제
		w = l->blink;
		if(w) w->link = NULL;
		else s = NULL;
		free(l);
		n--;
	}
	//leaf node인 경우
	if ((r->r == NULL) && (r->l == NULL)) { 
		w = s;
		int i = n - 1, cnt = 0, flag = 1;
		bitArr[r->c].n = 0;
		//bit정보를 10진수로 변환, 길이 정보 저장
		while (w) {
			if (w->n == true) bitArr[r->c].n += (int)pow(2, i);
			w = w->link;
			cnt++;
			i--;
		}
		bitArr[r->c].cnt = cnt;
		r->cnt = cnt;
		r->n = bitArr[r->c].n;
	}
}
//트리 동적할당 해제
void freeW(NodePtr r) {
	if (r->l) freeW(r->l);
	if (r->r) freeW(r->r);
	if (!r->l && !r->r) free(r);
}
//트리 정보 저장
//leaf node의 정보를 문자, 길이, 값의 순서로 저장
void fprintW(NodePtr r,FILE*fp) {
	if (r->l) fprintW(r->l,fp);
	if (r->r) fprintW(r->r,fp);
	if (!r->l && !r->r) {
		unsigned char temp[3] = { r->c,(unsigned char)r->cnt,(unsigned char)r->n };
		fwrite(&temp[0], sizeof(char), 1, fp);
		fwrite(&temp[1], sizeof(char), 1, fp);
		//값이 0인 경우 32와의 구분을 위해 0xFE를 넣어준다.
		if (temp[2] == 0) temp[2] = (unsigned char)0xFE;
		fwrite(&temp[2], sizeof(char), 1, fp);
	}
}
//파일압축
//트리 정보를 입력한 뒤 구분자를 넣고 파일정보를 8bit씩 입력->바이트코드의 길이가 8비트를 넘어가면 파일에 입력한다.
//남는 bit는 마지막을 0으로 채운 뒤 저장
void fileComp(char*filename,NodePtr bitArr,NodePtr root) {
	FILE*fpR = fopen(filename, "r");
	char filenameW[100] = "";
	strcpy(filenameW, filename);
	strcat(filenameW, ".zz");
	FILE*fpW = fopen(filenameW, "wb");
	//트리정보를 파일에 입력
	fprintW(root, fpW);
	//트리정보와 파일 정보의 구분을 위해서 아스키코드를 벗어나는 255를 저장
	unsigned char tmp = 0xFF;
	fwrite(&tmp, sizeof(char), 1, fpW);
	//파일 정보 저장
	char k;
	unsigned char temp[1] = { 0 };
	//val, cnt는 파일에서 읽어온 문자에 대한 정보
	//res, wb는 문자 정보에 대한 전체적인 관리
	//valF, cntF 종결문자에 대한 관리
	int val, cnt, res = 0, wb = 0, valF = bitArr[128].n, cntF = bitArr[128].cnt;
	while ((k = fgetc(fpR))!=EOF) {
		val = bitArr[k].n;
		cnt = bitArr[k].cnt;
		wb = (wb << cnt) + val;
		res += cnt;
		while (res > 7) {
			temp[0] = wb >> (res -= 8);
			fwrite(temp,sizeof(char),1,fpW);
		}
	}
	//마지막에 남은 bit처리
	if (res) {
		//남은 bit의 수와 종결문자 바이트 코드의 길이가 8bit을 넘는 경우
		//남은 bit뒤에 종결문자 바이트 코드를 채워주고 남은 bit을 입력한다.
		if (res + cntF > 8) {
			temp[0] = (wb << (8 - res)) + (valF >> (res + cntF - 8));
			fwrite(temp, sizeof(char), 1, fpW);
			temp[0] = (valF << (16 - res - cntF));
			fwrite(temp, sizeof(char), 1, fpW);
		}
		//8bit을 넘지 않는 경우
		else {
			temp[0] = (wb << (8 - res)) + (valF << (8 - res - cntF));
			fwrite(temp, sizeof(char), 1, fpW);
		}
	}
	//남은 bit이 없는 경우 종결문자 바이트 코드만 입력한다.
	else {
		temp[0] = valF << (8 - cntF);
		fwrite(temp, sizeof(char), 1, fpW);
	}
	fclose(fpR);
	fclose(fpW);
}
//읽어들인 문자 정보들을 가지고 트리 형성
//값과 길이로 길을 찾고 leaf node에 문자 저장
//0인 경우 왼쪽, 1인 경우 오른쪽으로 길 생성, 길이가 0인 경우 leaf node
void makeTreeD(NodePtr r,unsigned char* info) {
	//k는 1 또는 0, 1일 때 오른쪽 0일 때 왼쪽
	char k=(info[2]>>(--info[1]))&0x01;
	NodePtr temp = NULL;
	if (info[1] == 0) {
		temp = (NodePtr)malloc(sizeof(Node));
		temp->c = info[0];
		temp->r = NULL;
		temp->l = NULL;
		if (k) r->r = temp;
		else r->l = temp;
		return;
	}
	if (k) {
		if (r->r == NULL) {
			temp = (NodePtr)malloc(sizeof(Node));
			temp->r = NULL;
			temp->l = NULL;
			r->r = temp;
		}
		makeTreeD(r->r, info);
	}
	else {
		if (r->l == NULL) {
			temp = (NodePtr)malloc(sizeof(Node));
			temp->r = NULL;
			temp->l = NULL;
			r->l = temp;
		}
		makeTreeD(r->l, info);
	}
}
//파일에서 문자 정보에 대해 읽어들임
void readTreeD(NodePtr r, FILE*fp) {
	//문자와 해당하는 바이트코드 정보(문자, 길이, 값)저장 배열
	unsigned char info[3] = { 0 };
	//구분자에 대한 정보
	unsigned char finish = 0xFF, change = 0xFE;
	while (true) {
		//문자, 길이 ,값의 순서로 읽어들임
		fread(&info[0], sizeof(char), 1, fp);
		if (info[0] == finish) break;
		fread(&info[1], sizeof(char), 1, fp);
		fread(&info[2], sizeof(char), 1, fp);
		if (info[2] == change) info[2] = 0;
		makeTreeD(r, info);
	}
}

//8bit씩 읽어와서 각 bit값에 대해 허프만 트리를 이동하면서 leaf node인 경우 문자를 출력한다.
void writeFileD(NodePtr r, FILE*fpW, FILE*fpR) {
	NodePtr root = r;
	unsigned char temp, buffer[1] = { 0 };
	int i;
	while (true) {
		//8bit 읽어들임
		fread(buffer, sizeof(char), 1, fpR);
		for (i = 7; i >= 0; i--) {
			//앞의 bit부터 사용
			temp = (buffer[0] >> i)&0x01;
			//1인 경우 오른쪽, r인 경우 왼쪽으로 이동한다.
			if (temp) r = r->r;
			else r = r->l;
			//leaf node인 경우
			if (!r->l && !r->r) {
				//종결문자인 경우 출력을 종료한다.
				if (r->c == (unsigned char)0x80) return;
				//문자를 출력하고 r을 처음으로 이동시킨다.
				else {
					fprintf(fpW, "%c", r->c);
					r = root;
				}
			}
		}
	}
}
//압축해제
void fileDecomp(char*filename, NodePtr root) {
	char filenameW[100] = "";
	strcpy(filenameW, filename);
	strcat(filenameW, ".yy");
	FILE*fpR = fopen(filename, "rb");
	if (fpR == NULL) {
		printf("File not found");
		exit(1);
	}
	FILE*fpW = fopen(filenameW, "w");
	readTreeD(root, fpR);
	writeFileD(root,fpW,fpR);
}

int main(int argc, char* argv[]) {
	/*특별한 구분자==> 문자정보와 파일정보: 0xFF    0과 32(텍스트파일에서 모두 32로 입력):0xFE    종결문자:0x80(0~127까지의 문자 사용한다고 가정)*/

	//huffman coding에서 left, right에 따른 비트 저장
	NodePtr root = NULL;

	//////////////////////compression//////////////////////
	if (!strcmp(argv[1],"-c")) {
		//readArr: 파일을 읽어서 각 문자의 아스키 코드값에 해당하는 배열 값을 1증가시킴
		//heapArr: readArr중 0이 아닌 문자들을 min heap형태로 저장
		//bitArr: huffman coding을 이용해서 heap에 있는 문자들의 비트값에 따른 값을 저장
		Node readArr[128], heapArr[129], bitArr[129];
		//비트를 10진수로 저장할 때 사용
		BitPtr start, walk, last;
		start = walk = last = NULL;
		//size: heap tree의 크기, n: 비트의 길이
		int size, n = 0;
		readFile(argv[2], readArr);
		size = makeHeap(readArr, heapArr);
		root = makeTree(heapArr, size);
		for (int i = 0; i < 128; i++) bitArr[i].cnt = 0;
		getBit(bitArr, root, start, walk, last, n);
		fileComp(argv[2], bitArr, root);
		freeW(root);
	}

	//////////////////////decompression//////////////////////
	else if (!strcmp(argv[1],"-d")) {
		root = (NodePtr)malloc(sizeof(Node));
		root->r = NULL;
		root->l = NULL;
		fileDecomp(argv[2], root);
		freeW(root);
	}
	else {
		printf("wrong code is inserted\n");
		printf("end program\n");
		printf("compression: -c, decompression: -d");
		return -1;
	}
}