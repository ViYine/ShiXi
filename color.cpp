	void MultiInterfaceOnClourAlgorithm(Node* node,  MacDataUmsap* umsap, int n)
{
	int linkArray[100][100];            //存储链路关系的矩阵，等于1是对称邻居关系，等于0是非邻居关系
	int colourArray[100][100];        //存储着色情况的矩阵
	int slotArray[100][100];            //存储时隙分配情况的矩阵
	int interfaceArray[100][100];   //存储每条链路的outgoingInterface 0到14，15表示没有链路关系
	//int interColourArray[100][100];//存储每个节点着色之前已经被着色的情况
	//int coloured[100];                       //存储每个节点已经被着色和邻居节点已着色，用于后面着新色的比较
	int numNode=n;
	char addrString1[MAX_STRING_LENGTH];
	char addrString2[MAX_STRING_LENGTH];
	char* neighborInterfaceAddrStr;
	char* localInterfaceAddrStr;
	//初始化矩阵
	for (int i=1;i<n+1;i++)
	{
		for (int j=1;j<n+1;j++)
		{
			linkArray[i][j]=0;
			colourArray[i][j]=0;
			slotArray[i][j]=0;
			interfaceArray[i][j]=100;
			//interColourArray[i][j]=0;
		}
	}
	//根据链路关系构建邻接矩阵并且存储接口信息	
	PartitionData *partitionData = node->partitionData;
	Node *firstnode = partitionData->firstNode;
	Node* hashNode=  partitionData->firstNode;
	while(firstnode)
	{
		RoutingOlsr* olsr = (RoutingOlsr* ) firstnode->appData.olsr;
		int nodeId=firstnode->nodeId;
		link_entry* tmp_link_set;
		tmp_link_set = olsr->link_set;
		while(tmp_link_set)
		{
			//从本地节点的Neighbor iface addr : 190.0.3.8中切割出的nodeId=8，neighborInterface=3-1;
			//从本地节点的local iface addr : 190.0.10.4中切割出的nodeId=4，localInterface=10-1;
			IO_ConvertIpAddressToString(&tmp_link_set->neighbor_iface_addr, addrString1);
			IO_ConvertIpAddressToString(&tmp_link_set->local_iface_addr, addrString2);
			neighborInterfaceAddrStr=addrString1;
			localInterfaceAddrStr=addrString2;
			int split1[4];
			int split2[4];
			int *neighbor=SplitInterfaceAddrString(neighborInterfaceAddrStr,split1);
			int *local=SplitInterfaceAddrString(localInterfaceAddrStr,split2);
			int neighborId=neighbor[3];
			int neighborInterface=neighbor[2]-1;
			int localId=local[3];
			int localInterface=local[2]-1;	
			linkArray[localId][neighborId]=1;
			colourArray[localId][neighborId]=100;
			interfaceArray[localId][neighborId]=localInterface;
			tmp_link_set=tmp_link_set->next;
		}
		firstnode=firstnode->nextNodeData;
	}	
	//by zdz  2018/03/14  
    //计算节点的度并且排序
	multimap<int, int> nodeDegree;   //multimap按键从小到大排序，键可以不唯一
	for (int i=1;i<n+1;i++)
	{
		int d=0;
		for (int j=1;j<n+1;j++)
		{
			d=d+linkArray[i][j];
		}
		nodeDegree.insert(pair<int, int>(d,i));
	}
	//根据度迭代节点，准备着色
	for (multimap<int, int>::reverse_iterator iter= nodeDegree.rbegin(); iter != nodeDegree.rend(); iter++)
	{
		int degree=(*iter).first;
		int nodeId=(*iter).second;
		int numOnColour=0;
		int k=0;       //代表要比较的着色数
		//添加着色矩阵的已着色和邻居着色            //by zdz 2018/03/15
		map<int, int> coloured;
		map<int, int> mac1;
		map<int, int> mac2;
		map<int, int> mac3;
		//coloured.erase(coloured.begin(),coloured.end());
		for (int i=1;i<n+1;i++)
		{			
			if (colourArray[nodeId][i]!=100&&colourArray[nodeId][i]!=0)
			{
				coloured.insert(pair<int, int>(colourArray[nodeId][i],0));
			}
			else if (colourArray[nodeId][i]==100)
			{
				for (int j=1;j<n+1;j++)
				{
					if (colourArray[i][j]!=100&&colourArray[i][j]!=0)
					{
						coloured.insert(pair<int, int>(colourArray[i][j],0));
					}
				}
			}
		}
		//计算节点已经着色的链路数并且存储各个mac的着色情况
		for(int i=1;i<n+1;i++)
		{
			int interfaceColour;
			int colour;
			if (colourArray[nodeId][i]!=0&&colourArray[nodeId][i]!=100)
			{
				numOnColour++;
				interfaceColour=interfaceArray[nodeId][i]; 
				colour=colourArray[nodeId][i];
				macOnColour(mac1,mac2,mac3,interfaceColour,colour);
			}//end if
		}//end for

		//开始为节点的各个链路着色
		for (int i=1;i<n+1;i++)
		{
			if (colourArray[nodeId][i]==100)
			{
				bool isOk=false;
				int intf=interfaceArray[nodeId][i]; 
				colourArray[nodeId][i]=1;
				while(!isOk)
				{
					isOk=OnColourIsok(coloured,mac1,mac2,mac3,intf,colourArray[nodeId][i]);	//传参---不能同色的，包括已着色和邻居着色、包括颜色数	
					//传接口，比较中间数组的对应mac着色
					//传想要着的色号
					if (isOk)
					{
						numOnColour++;
						colourArray[i][nodeId]=colourArray[nodeId][i];
						slotArray[nodeId][i]=2*colourArray[nodeId][i];
						slotArray[i][nodeId]=2*colourArray[nodeId][i]-1;
						macOnColour(mac1,mac2,mac3,intf,colourArray[nodeId][i]);
						break;
					}
					else
					{
						colourArray[nodeId][i]+=1;
					}
				}				
			}
			if (numOnColour==degree)
			{
				break;
			}
		}  //当前节点着色完成
	}// 继续着第二个节点

	//by zdz 2018/03/16
	//准备分配时隙：分为以下两步
	 //1)查找所用的最大时隙数
	int maxSlot=0;
	map<int,int> numSlot;
	for (int i=1;i<n+1;i++){
		for (int j=1;j<n+1;j++){
			if (slotArray[i][j]!=0){
				numSlot.insert(pair<int, int>(slotArray[i][j],0));
			}
		}
	}
	maxSlot=numSlot.size();
	 //2)开始分配时隙
	int NFSlot=n*10;
	for (int i=1;i<n+1;i++)
	{
		for (int j=1;j<n+1;j++)
		{
			if (slotArray[i][j]!=0)
			{
				Node* currentNode=MAPPING_GetNodePtrFromHash(hashNode->partitionData->nodeIdHash,i);
				Node* recNode=MAPPING_GetNodePtrFromHash(hashNode->partitionData->nodeIdHash,j);
				int currentInterface=interfaceArray[i][j];
				int recInterface=interfaceArray[j][i];
			    MacDataUmsap* currentUmsap = (MacDataUmsap *)currentNode->macData[currentInterface]->macVar;
				MacDataUmsap* recUmsap = (MacDataUmsap *)recNode->macData[recInterface]->macVar;
				for (int m=NFSlot+slotArray[i][j];m<umsap->numSlotsPerFrame;m=m+maxSlot)
				{
					currentUmsap->frameDescriptor[m]=UMSAP_STATUS_TX;
					recUmsap->frameDescriptor[m]=UMSAP_STATUS_RX;
				}
			}
		}
	}//时隙分配完成
}
