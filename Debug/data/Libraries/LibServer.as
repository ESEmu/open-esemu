class Server
{
	Server()
	{
		
	}
	
	~Server()
	{
		
	}
	
	void setGlobalCache(string _field, string _value)
	{
		setCache(_field, _value);
	}
	
	string getGlobalCache(string _field)
	{
		string xout;
		getCache(_field, xout);
		return xout;
	}
}