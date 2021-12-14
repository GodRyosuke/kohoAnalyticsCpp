#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>


struct country_data {
	std::string country_name;
	int User;
	int Session;
	double pagePerSession;
};

struct month_data {
	int year_month;
	std::vector<country_data> countries;
	int User;
	int Session;
	double pagePerSession;
};

struct country_USP {
	std::string country_name;
	std::vector<int> User_year;
	std::vector<int> Session_year;
	std::vector<double> pagePerSessin_year;
};

int main(int argc, char** argv)
{
	std::cout << "hello" << std::endl;

	int file_name_num = 202012;
	std::vector<int> file_name_array = {
		202012,
		202101,
		202102,
		202103,
		202104,
		202105,
		202106,
		202107,
		202108,
		202109,
		202110,
		202111
	};

	std::vector <month_data> year_data;
	for (int i = 0; i < file_name_array.size(); i++) {
		std::string file_name = std::to_string(file_name_array[i]) + ".txt";

		month_data md;
		md.year_month = file_name_array[i];

		FILE* fp;  /* �t�@�C���|�C���^�̐錾 */
		char c;
		if ((fp = fopen(file_name.c_str(), "r")) == NULL) {  /* �t�@�C���̃I�[�v�� */
			printf("file open error!!\n");
			exit(1);
			return 1;
		}
		bool title_finish = false;
		int tub_count = 0;
		std::string country_name = "";
		std::string user_str = "";
		std::string session_str = "";
		std::string pagePerSession_str = "";
		while ((c = fgetc(fp)) != EOF) {
			putchar(c);
			if ((title_finish == false) && (c == '\n')) { // ���ڂ̉��s�Ȃ�e�[�u���g�b�v�I��
				title_finish = true;
				continue;
			}
			if (title_finish) {
				if (c == '\t') {
					tub_count++;
					continue;
				}
				if (c == '\n') { // 1�s�I���
					// ���̌��̂��̍��̃f�[�^�����o��
					country_data cd;
					cd.country_name = country_name;
					cd.User = std::atoi(user_str.c_str());
					cd.Session = std::atoi(session_str.c_str());
					cd.pagePerSession = std::atof(pagePerSession_str.c_str());
					md.countries.push_back(cd);

					country_name = "";
					user_str = "";
					session_str = "";
					pagePerSession_str = "";

					tub_count = 0;
					continue;
				}

				switch (tub_count) {
				case 0: 
					country_name += c;
					break;
				case 1:
					user_str += c;
					break;
				case 3:
					session_str += c;
					break;
				case 5:
					pagePerSession_str += c;
					break;
				}
			}
		}
		// �e���̍ŏI�s�̒l��month data��user�ɐݒ�
		country_data last_raw = md.countries[md.countries.size() - 1];
		md.User = last_raw.User;
		md.Session = last_raw.Session;
		md.pagePerSession = last_raw.pagePerSession;

		year_data.push_back(md);

		std::cout << "country: " + country_name << std::endl;
		std::cout << "user: " + user_str << std::endl;

		fclose(fp);
	}

	
	// ���݂��邷�ׂĂ̍��̃��X�g���쐬
	std::vector<std::string> country_list;
	for (auto md = year_data.begin(); md != year_data.end(); md++) {
		for (auto cd = md->countries.begin(); cd != md->countries.end(); cd++) {
			// ���ɍ��̃��X�g�ɓ����Ă��邩�`�F�b�N
			bool have_country = false;
			for (auto cl = country_list.begin(); cl != country_list.end(); cl++) {
				if (*cl == cd->country_name) {
					have_country = true;
				}
			}

			// �܂����̃��X�g�ɓ����Ă��Ȃ���Βǉ�
			if (!have_country) {
				country_list.push_back(cd->country_name);
			}
		}
	}

	// �e���A������User Session PV/Session�𓱏o
	std::vector<country_USP> output_array;
	for (auto cl = country_list.begin(); cl != country_list.end(); cl++) { // �S�Ă̍����Ƃɂ���
		std::string this_country_name = *cl;
		country_USP cU;
		cU.country_name = this_country_name;
		std::vector<int>user_year;
		std::vector<int>session_year;
		std::vector<double>sp_year;
		for (auto md = year_data.begin(); md != year_data.end(); md++) { // �e�����Ƃɂ���
			// �܂��A���̌���this_country_name�����邩�T��
			bool have_country = false;
			int user = 0;
			int session = 0;
			double sessionPerPgae = 0;
			for (auto cd = md->countries.begin(); cd != md->countries.end(); cd++) { // ���̌��̊e�����Ƃɂ���
				if (cd->country_name == this_country_name) {
					user = cd->User;
					session = cd->Session;
					sessionPerPgae = cd->pagePerSession;
					have_country = true;
				}
			}
			// ���̌���this_country���Ȃ���΁A0���A����΃f�[�^������B
			user_year.push_back(user);
			session_year.push_back(session);
			sp_year.push_back(sessionPerPgae);
		}
		cU.User_year = user_year;
		cU.Session_year = session_year;
		cU.pagePerSessin_year = sp_year;
		output_array.push_back(cU);
	}

	// �f�[�^�̌`����ύX
	std::vector<int> year_user; // �����Ƃ�user�̐������o��
	std::vector<int> year_session; // �����Ƃ�session�̐������o��
	std::vector<double> year_sp; // session per page ������
	for (int i = 0; i < 12; i++) {
		for (auto op = output_array.begin(); op != output_array.end(); op++) {
			year_user.push_back(op->User_year[i]);
			year_session.push_back(op->Session_year[i]);
			year_sp.push_back(op->pagePerSessin_year[i]);
		}
	}
	// �e�L�X�g�t�@�C���Ƃ��ďo��
	std::string output_file_name = "output.txt";
	FILE* fo;
	char c;
	if ((fo = fopen(output_file_name.c_str(), "w")) == NULL) {  /* �t�@�C���̃I�[�v�� */
		printf("file open error!!\n");
		return 1;
	}

	// 1�s��
	fputc('\t', fo);
	for (auto cl = country_list.begin(); cl != country_list.end(); cl++) {
		for (auto cc = cl->begin(); cc != cl->end(); cc++) {
			fputc(*cc, fo);
		}
		fputc('\t', fo);
		fputc('\t', fo);
	}
	fputc('\n', fo);

	/* fputc()�ɂ��o�� */
	//fputc(0x41, fp);
	//fputc(0x42, fp);
	//fputc(0x43, fp);
	//fputc(0x44, fp);
	//fputc(0x45, fp);
	//fputc(0xa, fp);


	return 0;
}