int subtree[N], parentcentroid[N];
vector<int> g[N];

void dfs(int k, int par)
{
	nodes++;
	subtree[k]=1;
	for(auto it:g[k])
	{
		if(it==par)
			continue;
		dfs(it, k);
		subtree[k]+=subtree[it];
	}
}

int centroid(int k, int par)
{
	for(auto it:g[k])
	{
		if(it==par)
			continue;
		if(subtree[it]>(nodes>>1))
			return centroid(it, k);
	}
	return k;
}

void decompose(int k, int par)
{
	nodes=0;
	dfs(k, k);
	int node=centroid(k, k);
	parentcentroid[node]=par;
	for(auto it:g[node])
	{
		g[it].erase(node);
		decompose(it, node);
	}
}
