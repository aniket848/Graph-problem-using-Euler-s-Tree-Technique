#include<bits/stdc++.h>
#include<iomanip>
using namespace std ;
#define pb push_back
#define ll long long int
#define vi vector<ll>  
#define VB vector<bool>
#define FOR(a, b) for (ll i = a; i < b; i++)
#define SORT(V) sort(V.begin(),V.end())
#define SIRVI ios_base::sync_with_stdio(false); cin.tie(NULL);
#define INF 1e18

#define MOD 1000000007

/***************************************  SOLVE **********************************************/

vector<ll> adj[100001];
ll val[100001],ft[200002],s[100001],t[100001];
ll nodeF[100001],eleF[100001];
ll LCA[100001][21];
ll res[100001];
ll level[100001];
ll blk=700;
ll timer=0,n,m;

struct query{
    ll l,r,c,idx,lca;
}Q[200002];



void dfs(ll node,ll par,ll lvl){ // DFS CALL TO FLATTEN THE TREE
     s[node] = timer; 
     ft[timer]=node; 
     timer++;
     LCA[node][0]=par;
     level[node]=lvl;

     for(auto child:adj[node]){
        if(child!=par){
            dfs(child,node,lvl+1);
        }
     }

     t[node]=timer; 
     ft[timer]=node; 
     timer++;
}

void init(){  //PREPROCESSING TREE USING BINARY LIFTING
   ll i,j;
    for(j=1;j<21;j++){
        for(i=1;i<=n;i++){
           if(LCA[i][j-1]!=-1){
              LCA[i][j] = LCA[LCA[i][j-1]][j-1];
           }
        }//i
    }//j
}

ll getLCA(ll a,ll b){  // RETURNING LCA OF TWO NODE

    if(level[a]>level[b])
        swap(a,b);

    ll d = level[b]-level[a];
    ll f=0;

    while(d){

        if(d&1){
            b = LCA[b][f];
        }
        f++;
        d=d/2;
    }

    if(a==b)
        return a;
    for(ll i=21-1;i>=0;i--){
        if(LCA[a][i]!=-1 && (LCA[a][i]!=LCA[b][i])){
            a=LCA[a][i] , b=LCA[b][i];
        }
    }

    return LCA[a][0];

}

ll getBlock(ll idx){ // RETURN BLOCK NO OF PARTICULAR INDEX
    return (idx+blk-1)/blk;
}

bool comp(query &q1 , query &q2){  // COMPARE FUNCTION ACCORDING TO MO'S ALGORITHM
    ll x = getBlock(q1.l);
    ll y = getBlock(q2.l);

    if(x!=y)
        return x<y;
    else
        return q1.r<q2.r;
}

void add(ll idx){  // ADD NODE IN OUR ANS

    ll node = ft[idx];
    nodeF[node]++;
    ll c= val[node];

    if(nodeF[node]==1){
        eleF[c]++;
    }else if(nodeF[node]==2){
         eleF[c]--;
    }
}

void remove(ll idx){  //REMOVE NODE FROM OUR ANS

    ll node = ft[idx];
    nodeF[node]--;
    ll c=val[node];

    if(nodeF[node]==1){
        eleF[c]++;
    }else{
       eleF[c]--;
    }
}

void solve(){
   ll test=0,i,j,k,x,y,m;

    while(cin>>n){

        for(i=0;i<100001;i++){ // INITIALISING VALUE TO ALL ARRAYS IN START OF EACH TEST CASE
            nodeF[i]=0;
            eleF[i]=0;level[i]=0;
            s[i]=0;
            t[i]=0;
            res[i]=0;
            adj[i].clear();
        }

        for(i=0;i<200002;i++)
            ft[i]=0;
        timer=1;
        memset(LCA,-1,sizeof(LCA));


        if(test>0)
            cout<<"\n";

        test++;
        cin>>m; //NO OF QUERIES

        for(i=1;i<=n;i++) // VALUE OF EACH NODE WHERE 'i' REPRESENT NODE AND VAL[i] REPRESENT ITS VALUE
            cin>>val[i];

        for(i=1;i<n;i++){ // TAKE THE TREE AS INPUT IN FORM OF EDGES
            cin>>x>>y;
            adj[x].pb(y);  adj[y].pb(x);     
        }

        dfs(1,-1,0); // FLATTEN THE TREE
        init(); // PREPROCESSING THE TREE TO GET LCA IN LOG(N) TIME

        ll a,b,c;
        for(i=1;i<=m;i++){
           cin>>a>>b>>c; 
           if(s[a]>s[b])
              swap(a,b);
           ll lca = getLCA(a,b);  //FIND LCA(LOWEST COMMON ANCESTOR) OF NODES 'A' AND 'B';

           if(a==lca)
             Q[i].l=s[a] , Q[i].r=s[b] , Q[i].idx=i , Q[i].lca=-1 , Q[i].c=c;
           else{
             Q[i].l=t[a] ; Q[i].r=s[b] ; Q[i].idx=i ; Q[i].lca =lca ; Q[i].c=c;
           }
        }//i

        sort(Q+1,Q+1+m,comp); // APPLYING MO'S ALGORITHM

        ll ML=1,MR=0;

        for(i=1;i<=m;i++){

            ll L=Q[i].l , R=Q[i].r , idx = Q[i].idx , lca=Q[i].lca , c=Q[i].c;

            while(MR<R)
              MR++,add(MR);
            while(MR>R)
                remove(MR),MR--;
            while(ML<L)
                remove(ML),ML++;
            while(ML>L)
                ML--,add(ML);

            if(eleF[c]>0)  // CHECK WHETHER WE FIND A NODE WITH VAULE 'C'
                res[idx]=1;
            else if(lca!=-1 && val[lca]==c) // CHECK WHETHER LCA IS A NODE WITH VALUE 'C' OR NOT
                res[idx]=1;
            else
                res[idx]=0;

        }

        for(i=1;i<=m;i++){  //PRINT OUTPUT
            if(res[i])
                cout<<"Find"<<"\n";
            else
                cout<<"NotFind"<<"\n";
        }
    }//while

}


int main()
{
    SIRVI ; // FOR FAST INPUT OUTPUT
    solve();
    return 0 ; 
}

