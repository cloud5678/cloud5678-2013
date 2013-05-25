int fmin(int v1,int v2){
  if(v1>v2)return v2;
  else return v1;
}
int fmax(int v1,int v2){
  if(v1>v2)return v1;
  else return v2;
}
int sign(int in){
	if(in !=0) return in/abs(in);
	else return 0;
}
