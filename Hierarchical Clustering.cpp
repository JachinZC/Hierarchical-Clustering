#include"Hierarchical Clustering.h"

class Node //点集
{
public:
	float N_x;
	float N_y;
	//vector<Cluster> C; //簇的数组集合，同一簇的点放入一个簇集C中
	Node(float x, float y)
	{
		N_x = x;
		N_y = y;
		//cluster n(x,y); //刚开始每个点都形成一个簇
		//c.push_back(n);
	}
};

class Cluster //簇集
{
public:
	float C_x;
	float C_y;
	vector<Node> NodeInCluster; //簇中包含的点
	Cluster(float x, float y)
	{
		C_x = x;
		C_y = y;
		Node n(x, y);
		NodeInCluster.push_back(n);
	}
};

float GetDistance(Cluster a, Cluster b) //计算每簇点之间的距离
{
	return sqrt((a.C_x - b.C_x) * (a.C_x - b.C_x)) + (a.C_y - b.C_y) * (a.C_y - b.C_y);
}


float StringToFloat(string str)//用于读文件时，将string类型数据转化成float类型
{
	stringstream sf;
	float f = 0;
	sf << str;
	sf >> f;
	return f;
}

vector<Cluster> GetFile(const char* database) //读取txt文件中的点的信息（x，y）修改点！！！，返回的是一个（Cluster1，Cluster2...）的数组
{
	ifstream ifs;
	ifs.open(database, ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败！" << endl;
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
	cout << "数据读取成功！" << endl;
	ifs.close();
	return data;
}

vector<vector<float>> DistanceMatrix(vector<Cluster> dataset, vector<vector<float>> Dst) //更新簇距离矩阵
{
	int ClusterNum = dataset.size();
	cout << "目前簇数: " << ClusterNum << endl;
	for (int i = 0; i < ClusterNum; i++)
	{
		vector<float> temp;
		for (int j = 0; j < ClusterNum; j++)
		{
			if (i < j)//列数小于行数时，储存距离，只储存在左下
			{
				temp.push_back(GetDistance(dataset[i], dataset[j]));
			}
			else temp.push_back(0);
		}
		Dst.push_back(temp);
		return Dst;
	}
}

void OutputResult(vector<Cluster> dataset) //输出结果
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

void Hierarchical_Clustering(vector<Cluster> dataset,int FinalC_num) //首次迭代dataset就是GetFile的返回值
{
	vector<vector<float>> Dst;  //二维数组，用于储存各簇（首次迭代为点）之间的距离
	Dst = DistanceMatrix(dataset, Dst); //初始化距离矩阵
	while (dataset.size() > FinalC_num)
	{
		float MinDst = INT_MAX;
		int Min_i, Min_j; //用于记录最小两簇的索引
		for (int i = 0; i < Dst.size(); i++) //遍历的方法找到距离最小的两簇
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
		for (int i = 0; i < dataset[Min_i].NodeInCluster.size(); i++) //合并两簇并更新新簇的平均坐标
		{
			dataset[Min_j].C_x = (dataset[Min_j].C_x + dataset[Min_i].C_x) / 2;
			dataset[Min_j].C_y = (dataset[Min_j].C_y + dataset[Min_i].C_y) / 2;
			dataset[Min_j].NodeInCluster.push_back(dataset[Min_i].NodeInCluster[i]);
		}
		dataset.erase(dataset.begin() + Min_i); //删除被合并簇
		Dst = DistanceMatrix(dataset, Dst); //更新簇距离矩阵
	}
	OutputResult(dataset);
	cout << "聚类完毕！" << endl;
}

int main()
{
	vector<Cluster> data = GetFile("dataset3.txt");
	Hierarchical_Clustering(data, 3);
	//cout << data[3].C_y << endl;
	system("pause");
	return 0;
}