#include "Z80.h"
#include <stdio.h>
#include <stdlib.h>


#define	MAX_MEMORY			(32 * 1024)		/* �g���郁���� */


#ifndef NULL
#define	NULL		(0)
#endif
#ifndef	FALSE
#define	FALSE		(0)
#define	TRUE		(1)
#endif

/// �t�@�C���̓ǂݍ���
void* load(const char* fn, int* psize) {
	void* data = NULL;
	FILE* fp = fopen(fn, "rb");
	if (fp != NULL) {
		int size;
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		data = malloc(size);
		if (data != NULL) {
			fread(data, 1, size, fp);
			if (psize != NULL) {
				*psize = size;
			}
		}

		fclose(fp);
	}
	return data;
}

/// �ǂݍ��񂾃f�[�^�̉��
void release_load_data(void* data) {
	free(data);
}

//=============================================================================
/// Z80 �G�~���̃h���C�u

                               /* Bits in Z80 F register:    */
#define S_FLAG      0x80       /* 1: Result negative         */
#define Z_FLAG      0x40       /* 1: Result is zero          */
#define H_FLAG      0x10       /* 1: Halfcarry/Halfborrow    */
#define P_FLAG      0x04       /* 1: Result is even          */
#define V_FLAG      0x04       /* 1: Overflow occured        */
#define N_FLAG      0x02       /* 1: Subtraction occured     */
#define C_FLAG      0x01       /* 1: Carry/Borrow occured    */

#define M_RET R->PC.B.l=RdZ80(R->SP.W++);R->PC.B.h=RdZ80(R->SP.W++)

/// �G�~�����[�^
Z80 z80;

/// ���������
byte mem[64 * 1024];

/// ����������P�o�C�g�ǂݍ���
byte RdZ80(word A) {
	return mem[A];
}

/// �������ɏ�������
void WrZ80(word Addr, byte Value) {
	mem[Addr] = Value;
}

/// I/O �o��
void OutZ80(word Port, byte Value) {
}

/// I/O ����
byte InZ80(word Port) {
	return 0;
}

void PatchZ80(register Z80 *R)		{ }
word LoopZ80(register Z80 *R)		{ return INT_QUIT; }



/// ������
void init() {
	ResetZ80(&z80);
}

/// �������ւ̓]��
void trans(word adr, const void* data, int size) {
	const char* tbl = (const char*)data;
	int i;
	if (size >= MAX_MEMORY) {
		size = MAX_MEMORY;
	}
	for (i=0; i<size; ++i) {
		WrZ80(adr+i, tbl[i]);
	}
}

/// ��~�ς݁H
int is_halt() {
	return z80.IFF & IFF_HALT;
}


//=============================================================================

/// ���^�[������
void ret() {
	Z80* R = &z80;
	M_RET;
}

/// �P�����o��
void _putchar() {
	int c = z80.AF.B.h;
	putchar(c);
	ret();
}

/// �P��������
void _getchar() {
	int c = getchar();
	if (c != EOF) {
		z80.AF.B.h = c;
		z80.AF.B.l &= ~C_FLAG;
	} else {
		z80.AF.B.l |= C_FLAG;
	}
	ret();
}

/// ���ʃA�h���X�֐��e�[�u��
struct {
	word adr;
	void (*func)();
} static const functbl[] = {
	{	0x8000,		_putchar,	},
	{	0x8003,		_getchar,	},
};

/// �P���ߎ��s
void step() {
	ExecZ80(&z80);

		// �V�X�e���R�[�����ꂽ�ꍇ�A���̏����֔�΂�
	{
		const int N = sizeof(functbl)/sizeof(*functbl);
		int i;
		for (i=0; i<N; ++i) {
			if (z80.PC.W == functbl[i].adr) {
				(*functbl[i].func)();
				break;
			}
		}
	}
}


//=============================================================================

void exec_loop(int maxstep) {
	int i;
	for (i=0; maxstep == 0 || i < maxstep; ++i) {
		if (is_halt())	break;
		step();
	}
}

/// �t�@�C�����s
int exec(const char* fn, int maxstep) {
	int size;
	void* data = load(fn, &size);
	if (data == NULL) {
		fprintf(stderr, "%s: load err\n", fn);
		return FALSE;
	}

	trans(0x0000, data, size);
	release_load_data(data);

	exec_loop(maxstep);

	if (!is_halt()) {
		fprintf(stderr, "time out!\n");
		return FALSE;
	}

	return TRUE;
}


void usage()
{
	fprintf(stdout,
		"usage: [option] z80-source-file\n"
		"option:\n"
		"	-h          help\n"
		"	-s<step>    set max step num\n"
	);
}

const int ERR = 1;

/// �G���g��
int main(int argc, char* argv[])
{
	int maxstep = 0;
	int i;

	for (i=1; i<argc; ++i) {
		const char* p = argv[i];
		if (*p != '-')	break;
		switch (p[1]) {
		default:
			fprintf(stderr, "%c: illegal option\n", p[1]);
			return ERR;
		case 'h':
			usage();
			break;
		case 's':
			maxstep = atoi(p + 2);
			break;
		}
	}

	if (i >= argc) {
		usage();
		return ERR;
	}

	if (!exec(argv[i], maxstep)) {
		return ERR;
	}

	return 0;
}

