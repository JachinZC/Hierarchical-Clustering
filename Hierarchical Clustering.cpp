#include"Hierarchical Clustering.h"

class Node //�㼯
{
public:
	float N_x;
	float N_y;
	//vector<Cluster> C; //�ص����鼯�ϣ�ͬһ�صĵ����һ���ؼ�C��
	Node(float x, float y)
	{
		N_x = x;
		N_y = y;
		//cluster n(x,y); //�տ�ʼÿ���㶼�γ�һ����
		//c.push_back(n);
	}
};

class Cluster //�ؼ�
{
public:
	float C_x;
	float C_y;
	vector<Node> NodeInCluster; //���а����ĵ�
	Cluster(float x, float y)
	{
		C_x = x;
		C_y = y;
		Node n(x, y);
		NodeInCluster.push_back(n);
	}
};

float GetDistance(Cluster a, Cluster b) //����ÿ�ص�֮��ľ���
{
	return sqrt((a.C_x - b.C_x) * (a.C_x - b.C_x)) + (a.C_y - b.C_y) * (a.C_y - b.C_y);
}


float StringToFloat(string str)//���ڶ��ļ�ʱ����string��������ת����float����
{
	stringstream sf;
	float f = 0;
	sf << str;
	sf >> f;
	return f;
}

vector<Cluster> GetFile(const char* database) //��ȡtxt�ļ��еĵ����Ϣ��x��y���޸ĵ㣡���������ص���һ����Cluster1��Cluster2...��������
{
	ifstream ifs;
	ifs.open(database, ios::in);
	if (!ifs.is_open())
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		vector<Cluster> a;
		return a;
	}
	vector<Cluster> data;
	string buf;
	while (getline(ifs,buf))
	{
		int pos = buf.find(",", 0);
		//cout<<pos<<" ";
		//cout << StringToFloat(buf.substr(pos + 1))<<" ";
		Cluster c(StringToFloat(buf.substr(0, pos)), StringToFloat(buf.substr(pos + 1)));
		data.push_back(c);
	}
	cout << "���ݶ�ȡ�ɹ���" << endl;
	ifs.close();
	return data;
}

vector<vector<float>> DistanceMatrix(vector<Cluster> dataset, vector<vector<float>> Dst) //���´ؾ������
{
	int ClusterNum = dataset.size();
	cout << "Ŀǰ����: " << ClusterNum << endl;
	for (int i = 0; i < ClusterNum; i++)
	{
		vector<float> temp;
		for (int j = 0; j < ClusterNum; j++)
		{
			if (i < j)//����С������ʱ��������룬ֻ����������
			{
				temp.push_back(GetDistance(dataset[i], dataset[j]));
			}
			else temp.push_back(0);
		}
		Dst.push_back(temp);
		return Dst;
	}
}

void OutputResult(vector<Cluster> dataset) //������
{
	ofstream ofs;
	ofs.open("clustering.txt", ios::out);
	for (int i = 0; i < dataset.size(); i++)
	{
		for (int j = 0; j < dataset[i].NodeInCluster.size(); j++)
		{
			ofs << dataset[i].NodeInCluster[j].N_x << "\t" << dataset[i].NodeInCluster[j].N_y << "\t" << i + 1 << endl;
		}
	}
	ofs.close();
}

void Hierarchical_Clustering(vector<Cluster> dataset,int FinalC_num) //�״ε���dataset����GetFile�ķ���ֵ
{
	vector<vector<float>> Dst;  //��ά���飬���ڴ�����أ��״ε���Ϊ�㣩֮��ľ���
	Dst = DistanceMatrix(dataset, Dst); //��ʼ���������
	while (dataset.size() > FinalC_num)
	{
		float MinDst = INT_MAX;
		int Min_i, Min_j; //���ڼ�¼��С���ص�����
		for (int i = 0; i < Dst.size(); i++) //�����ķ����ҵ�������С������
		{
			for (int j = i + 1; j < Dst[i].size(); j++)
			{
				if (Dst[i][j] < MinDst)
				{
					Min_i = i;
					Min_j = j;
					MinDst = Dst[i][j];
				}
			}
		}
		for (int i = 0; i < dataset[Min_i].NodeInCluster.size(); i++) //�ϲ����ز������´ص�ƽ������
		{
			dataset[Min_j].C_x = (dataset[Min_j].C_x + dataset[Min_i].C_x) / 2;
			dataset[Min_j].C_y = (dataset[Min_j].C_y + dataset[Min_i].C_y) / 2;
			dataset[Min_j].NodeInCluster.push_back(dataset[Min_i].NodeInCluster[i]);
		}
		dataset.erase(dataset.begin() + Min_i); //ɾ�����ϲ���
		Dst = DistanceMatrix(dataset, Dst); //���´ؾ������
	}
	OutputResult(dataset);
	cout << "������ϣ�" << endl;
}

int main()
{
	vector<Cluster> data = GetFile("dataset3.txt");
	Hierarchical_Clustering(data, 3);
	//cout << data[3].C_y << endl;
	system("pause");
	return 0;
}