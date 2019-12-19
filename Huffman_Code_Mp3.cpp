#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//Ʈ���� �迭�� �������
typedef struct Node* NodePtr;
typedef struct Node{
unsigned char c;
int n, cnt;
NodePtr l, r;
}Node;

//����Ʈ �ڵ� ������ �ʿ��� Ʈ���� ���� �� ���
typedef struct Bit* BitPtr;
typedef struct Bit {
	bool n;
	BitPtr link, blink;  //link: ������ ����, blink: �ڷ� ����
}Bit;

//���� ���� �о����
//���ڸ��� ������ ���� �����ؼ� �迭�� ����
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
//min heap���� ���� Ƚ���� ���� ������ �°� ��带 �����Ѵ�.
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
//���Ͽ� �� ���̶� ��Ÿ���ٸ� heap�� �־��ش�.
//heapArr[1]�� ���Ṯ�ڿ� ���� ����(Ƚ���� 0�̹Ƿ� ù��°�� �����Ѵ�)
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
//heap���� ����Ƚ���� ���� ���� ��带 ��ȯ�ϰ� ���������ش�.
//n�� heap�� size
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
//����Ʈ �ڵ� ������ �ʿ��� Ʈ���� ������ش�.
//min heap���� ũ�Ⱑ ���� �ͺ��� �ҷ��� ���� Ʈ���� ������� �� heap�� �ٽ� �ִ´�.
//heap�� size-1��ŭ �ݺ��Ѵ�.
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
//Ʈ���� �ִ� ��Ʈ ������ 10������ �迭�� ����
//010���� ���� �˾Ƽ��� �� �Ǵ� ��� �����ؼ� ���̵� ����
void getBit(NodePtr bitArr, NodePtr r, BitPtr s, BitPtr w, BitPtr l, int n) {
	/*
	s,w,l�� tree�� �̵��Կ� ���� �߰��Ǵ� ��Ʈ�� ������ ex)root�κ��� left right left��� 010�� �����ؾ��ϰ� s=0,l=0�� �����ϰ�����
	��Ʈ�� �ڸ����� ���� bitArr�� ������ 010�� ��� ���� 2���� ���� 0�� �����ؾ��ϱ� ������ �ڸ��� 3�� �����ϰ� ���߿� ó����
	n�� s~l�� ������ ��Ʈ�� ���� ���� ������ �� ���� ������ ���� ����. ex)101��Ʈ�� ������ �� n�� 3�̰� ó�� 1���� 2�� n-1���� ���������.
	*/
	if (r->l != NULL) {
		//��ü Ʈ���� root��忡�� �������� �� ��
		if (s == NULL) {
			s = (BitPtr)malloc(sizeof(Bit));
			s->n = false;
			s->link = NULL;
			s->blink = NULL;
			l = s;
			w = s;
		}
		//0bit�� ���� ������ �����ϰ� �������� �̵�
		else {
			l = (BitPtr)malloc(sizeof(Bit));
			l->n = false;
			l->link = NULL;
			l->blink = w;
			w->link = l;
			w = l;
		}
		getBit(bitArr, r->l, s, w, l, ++n);
		//����� 0bit�� ���� ���� ����
		w = l->blink;
		if(w) w->link = NULL;
		else s = NULL;
		free(l);
		n--;
	}
	if (r->r != NULL) {
		//��ü Ʈ���� root��忡�� ���������� �� ��
		if (s == NULL) {
			s = (BitPtr)malloc(sizeof(Bit));
			s->n = true;
			s->link = NULL;
			s->blink = NULL;
			l = s;
			w = s;
		}
		//1bit�� ���� ���� �����ϰ� ���������� �̵�
		else {
			l = (BitPtr)malloc(sizeof(Bit));
			l->n = true;
			l->link = NULL;
			l->blink = w;
			w->link = l;
			w = l;
		}
		getBit(bitArr, r->r, s, w, l, ++n);
		//����� 1bit�� ���� ���� ����
		w = l->blink;
		if(w) w->link = NULL;
		else s = NULL;
		free(l);
		n--;
	}
	//leaf node�� ���
	if ((r->r == NULL) && (r->l == NULL)) { 
		w = s;
		int i = n - 1, cnt = 0, flag = 1;
		bitArr[r->c].n = 0;
		//bit������ 10������ ��ȯ, ���� ���� ����
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
//Ʈ�� �����Ҵ� ����
void freeW(NodePtr r) {
	if (r->l) freeW(r->l);
	if (r->r) freeW(r->r);
	if (!r->l && !r->r) free(r);
}
//Ʈ�� ���� ����
//leaf node�� ������ ����, ����, ���� ������ ����
void fprintW(NodePtr r,FILE*fp) {
	if (r->l) fprintW(r->l,fp);
	if (r->r) fprintW(r->r,fp);
	if (!r->l && !r->r) {
		unsigned char temp[3] = { r->c,(unsigned char)r->cnt,(unsigned char)r->n };
		fwrite(&temp[0], sizeof(char), 1, fp);
		fwrite(&temp[1], sizeof(char), 1, fp);
		//���� 0�� ��� 32���� ������ ���� 0xFE�� �־��ش�.
		if (temp[2] == 0) temp[2] = (unsigned char)0xFE;
		fwrite(&temp[2], sizeof(char), 1, fp);
	}
}
//���Ͼ���
//Ʈ�� ������ �Է��� �� �����ڸ� �ְ� ���������� 8bit�� �Է�->����Ʈ�ڵ��� ���̰� 8��Ʈ�� �Ѿ�� ���Ͽ� �Է��Ѵ�.
//���� bit�� �������� 0���� ä�� �� ����
void fileComp(char*filename,NodePtr bitArr,NodePtr root) {
	FILE*fpR = fopen(filename, "r");
	char filenameW[100] = "";
	strcpy(filenameW, filename);
	strcat(filenameW, ".zz");
	FILE*fpW = fopen(filenameW, "wb");
	//Ʈ�������� ���Ͽ� �Է�
	fprintW(root, fpW);
	//Ʈ�������� ���� ������ ������ ���ؼ� �ƽ�Ű�ڵ带 ����� 255�� ����
	unsigned char tmp = 0xFF;
	fwrite(&tmp, sizeof(char), 1, fpW);
	//���� ���� ����
	char k;
	unsigned char temp[1] = { 0 };
	//val, cnt�� ���Ͽ��� �о�� ���ڿ� ���� ����
	//res, wb�� ���� ������ ���� ��ü���� ����
	//valF, cntF ���Ṯ�ڿ� ���� ����
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
	//�������� ���� bitó��
	if (res) {
		//���� bit�� ���� ���Ṯ�� ����Ʈ �ڵ��� ���̰� 8bit�� �Ѵ� ���
		//���� bit�ڿ� ���Ṯ�� ����Ʈ �ڵ带 ä���ְ� ���� bit�� �Է��Ѵ�.
		if (res + cntF > 8) {
			temp[0] = (wb << (8 - res)) + (valF >> (res + cntF - 8));
			fwrite(temp, sizeof(char), 1, fpW);
			temp[0] = (valF << (16 - res - cntF));
			fwrite(temp, sizeof(char), 1, fpW);
		}
		//8bit�� ���� �ʴ� ���
		else {
			temp[0] = (wb << (8 - res)) + (valF << (8 - res - cntF));
			fwrite(temp, sizeof(char), 1, fpW);
		}
	}
	//���� bit�� ���� ��� ���Ṯ�� ����Ʈ �ڵ常 �Է��Ѵ�.
	else {
		temp[0] = valF << (8 - cntF);
		fwrite(temp, sizeof(char), 1, fpW);
	}
	fclose(fpR);
	fclose(fpW);
}
//�о���� ���� �������� ������ Ʈ�� ����
//���� ���̷� ���� ã�� leaf node�� ���� ����
//0�� ��� ����, 1�� ��� ���������� �� ����, ���̰� 0�� ��� leaf node
void makeTreeD(NodePtr r,unsigned char* info) {
	//k�� 1 �Ǵ� 0, 1�� �� ������ 0�� �� ����
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
//���Ͽ��� ���� ������ ���� �о����
void readTreeD(NodePtr r, FILE*fp) {
	//���ڿ� �ش��ϴ� ����Ʈ�ڵ� ����(����, ����, ��)���� �迭
	unsigned char info[3] = { 0 };
	//�����ڿ� ���� ����
	unsigned char finish = 0xFF, change = 0xFE;
	while (true) {
		//����, ���� ,���� ������ �о����
		fread(&info[0], sizeof(char), 1, fp);
		if (info[0] == finish) break;
		fread(&info[1], sizeof(char), 1, fp);
		fread(&info[2], sizeof(char), 1, fp);
		if (info[2] == change) info[2] = 0;
		makeTreeD(r, info);
	}
}

//8bit�� �о�ͼ� �� bit���� ���� ������ Ʈ���� �̵��ϸ鼭 leaf node�� ��� ���ڸ� ����Ѵ�.
void writeFileD(NodePtr r, FILE*fpW, FILE*fpR) {
	NodePtr root = r;
	unsigned char temp, buffer[1] = { 0 };
	int i;
	while (true) {
		//8bit �о����
		fread(buffer, sizeof(char), 1, fpR);
		for (i = 7; i >= 0; i--) {
			//���� bit���� ���
			temp = (buffer[0] >> i)&0x01;
			//1�� ��� ������, r�� ��� �������� �̵��Ѵ�.
			if (temp) r = r->r;
			else r = r->l;
			//leaf node�� ���
			if (!r->l && !r->r) {
				//���Ṯ���� ��� ����� �����Ѵ�.
				if (r->c == (unsigned char)0x80) return;
				//���ڸ� ����ϰ� r�� ó������ �̵���Ų��.
				else {
					fprintf(fpW, "%c", r->c);
					r = root;
				}
			}
		}
	}
}
//��������
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
	/*Ư���� ������==> ���������� ��������: 0xFF    0�� 32(�ؽ�Ʈ���Ͽ��� ��� 32�� �Է�):0xFE    ���Ṯ��:0x80(0~127������ ���� ����Ѵٰ� ����)*/

	//huffman coding���� left, right�� ���� ��Ʈ ����
	NodePtr root = NULL;

	//////////////////////compression//////////////////////
	if (!strcmp(argv[1],"-c")) {
		//readArr: ������ �о �� ������ �ƽ�Ű �ڵ尪�� �ش��ϴ� �迭 ���� 1������Ŵ
		//heapArr: readArr�� 0�� �ƴ� ���ڵ��� min heap���·� ����
		//bitArr: huffman coding�� �̿��ؼ� heap�� �ִ� ���ڵ��� ��Ʈ���� ���� ���� ����
		Node readArr[128], heapArr[129], bitArr[129];
		//��Ʈ�� 10������ ������ �� ���
		BitPtr start, walk, last;
		start = walk = last = NULL;
		//size: heap tree�� ũ��, n: ��Ʈ�� ����
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