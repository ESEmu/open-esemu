class Database
{
	string execQuery(string &in a)
	{
		string result = "";
		executeQuery(a, result);
		return result;
	}
}