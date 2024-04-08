#include "CollectClassInfo.h"

namespace CollectClassInfo
{
	constexpr char SpaceString[] = " ";
	constexpr char ColonString[] = ":";
	constexpr char CommaString[] = ",";
	constexpr char LeftParenthesisString[] = "(";
	constexpr char StarString[] = "*";
	constexpr char FetchAddressString[] = "&";
	constexpr char CodeType[] = "CodeType";

	bool GetCodeTypeByFunc(
		const string& RowStrong, 
		FFunctionAnalysis& FunctionAnalysis)
	{
		const char* Ptr = const_cast<char*>(RowStrong.c_str());

		char R[1024] = { 0 };
		char L[1024] = { 0 };
		//UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
		//L =  "Describe", Group = "SimpleCodeLibrary"))
		//R = UFUNCTION(Meta = (

		split(Ptr, CodeType, R, L, false);

		vector<string> ElementStr;
		simple_cpp_string_algorithm::parse_into_vector_array(L, ElementStr, ",");

		if (simple_cpp_string_algorithm::string_contain(ElementStr[0], "Event"))
		{
			FunctionAnalysis.CodeType = "Event";

			return true;
		}
		else if (simple_cpp_string_algorithm::string_contain(ElementStr[0], "Function"))
		{
			FunctionAnalysis.CodeType = "Function";

			return true;
		}
		else if (simple_cpp_string_algorithm::string_contain(ElementStr[0], "PureFunction"))
		{
			FunctionAnalysis.CodeType = "PureFunction";

			return true;
		}
		return false;
	}

	FParamElement CollectionVariableType(
		const std::string& Variable,
		ECollectionParmType CollectionParmType = ECollectionParmType::Type_Parm)
	{
		char* Ptr = const_cast<char*>(Variable.c_str());

		//int & 
		FParamElement ParamElement;
		ParamElement.Name = "ReturnValue";

		if (simple_cpp_string_algorithm::string_contain(Variable, StarString))
		{
			remove_char_end(Ptr, '*');
			ParamElement.bPointer = true;
		}

		if (simple_cpp_string_algorithm::string_contain(Variable, FetchAddressString))
		{
			remove_char_end(Ptr, '&');
			ParamElement.bReference = true;
		}

		//int  
		if (simple_cpp_string_algorithm::string_contain(Variable, "const"))
		{
			trim_start_and_end_inline(Ptr);

			remove_string_start(Ptr, "const");
			ParamElement.bConst = true;
		}

		trim_start_and_end_inline(Ptr);

		remove_char_start(const_cast<char*>(Variable.c_str()), '\t');
		ParamElement.Type = Variable;

		return ParamElement;
	}

	bool GetCodeTypeByProp(
		const string& RowStrong,
		FVariableAnalysis* VariableAnalysis)
	{
		//RowStrong = UPROPERTY(Meta = (CodeType = "Resources", Group = "SimpleCodeLibrary"))
	
		char R[1024] = { 0 };
		char L[1024] = { 0 };

		const char* Ptr = const_cast<char*>(RowStrong.c_str());
		split(Ptr, CodeType, R, L, false);

		//R =  UPROPERTY(Meta = (
		//L =  = "Resources", Group = "SimpleCodeLibrary"))

		vector<string> ElementStr;
		simple_cpp_string_algorithm::parse_into_vector_array(L, ElementStr, CommaString);
	
		// "Resources" 0 
		// Group = "SimpleCodeLibrary")) 1
		if (simple_cpp_string_algorithm::string_contain(ElementStr[0], "Resources"))
		{
			VariableAnalysis->CodeType = "Resources";
			return true;
		}

		return false;
	}

	//Vector123124<float,int,float,int> float,int,float,int xx,xx,xx,xx
	//void XXXX(xx,xx,xx,xx)?
	void AnalyticParameters(char* LStr, std::vector<FParamElement>& OutParam)
	{
		//解析参数和参数名
		vector<string> ElementStr;
		simple_cpp_string_algorithm::parse_into_vector_array(LStr, ElementStr, CommaString);

		//UObject *Context
		//int32 &A
		//float b
		//bool C

		//收集变量
		for (std::string& Ele : ElementStr)
		{
			char* ElePtr = const_cast<char*>(Ele.c_str());

			if (Ele == "")
			{
				continue;
			}

			//int32 &A
			//移除前后空格
			trim_start_and_end_inline(ElePtr);

			OutParam.push_back(FParamElement());
			FParamElement &ParamElement = OutParam[OutParam.size() - 1];

			char R[1024] = { 0 };
			char L[1024] = { 0 };
			if (simple_cpp_string_algorithm::string_contain(Ele, StarString))
			{
				ParamElement.bPointer = true;
				//GObject *Context
				split(ElePtr, StarString, R, L, false);
				//R = GObject
				//L = Context
			}
			else if (simple_cpp_string_algorithm::string_contain(Ele, FetchAddressString))
			{
				ParamElement.bReference = true;
				split(ElePtr, FetchAddressString, R, L, false);
			}
			else
			{
				//  int a
				split(ElePtr, SpaceString, R, L, false);
				if (R[0] == '\0')
				{
					strcpy(R, ElePtr);
				}
			}

			if (c_str_contain(R, "const"))
			{
				//const GObject *Context
				ParamElement.bConst = true;

				remove_string_start(R, "const");
			}

			trim_start_and_end_inline(R);
			trim_start_and_end_inline(L);

			ParamElement.Type = R;
			ParamElement.Name = L;
		}
	}

	bool Collection(const string& Paths, FClassAnalysis& ClassAnalysis)
	{
		std::vector<std::string> StringArray;
		simple_cpp_helper_file::load_file_to_strings(Paths, StringArray);

		//收集filname
		ClassAnalysis.Filename = Paths;

		//遍历每一行代码
		for (int i = 0; i < StringArray.size(); i++)
		{
			string& Row = StringArray[i];
			char* RowPtr = const_cast<char*>(Row.c_str());

			//包含
			auto Contain = [&](const char* InSubString) -> bool
			{
				return simple_cpp_string_algorithm::string_contain(Row, InSubString);
			};

			if (Contain("CODEREFLECTION"))
			{
				ClassAnalysis.CodeLine = i + 1;
			}

			//获取类名和继承名
			if ((Contain("\tclass") || Contain("class")) &&
				Contain(":") &&
				(Contain("protected") || Contain("private") || Contain("public")))
			{
				//class FRenderingPipeline :public IDirectXDeviceInterface
				//FRenderingPipeline :public IDirectXDeviceInterface
				remove_string_start(RowPtr, "class ");
				remove_string_start(RowPtr, "\tclass ");

				if (Contain("_API"))
				{
					//XXX_API FRenderingPipeline :public IDirectXDeviceInterface
					//L ="XXX_API" R = " FRenderingPipeline :public IDirectXDeviceInterface"
					trim_start_inline(RowPtr);

					char R[1024] = { 0 };
					char L[1024] = { 0 };

					split(RowPtr, SpaceString, L, R, false);

					//API名称
					ClassAnalysis.APIName = L;

					Row = R;
				}

				vector<string> ElementStr;
				simple_cpp_string_algorithm::parse_into_vector_array(RowPtr, ElementStr, ColonString);
			
				//如果前后有空格 就把它修剪了
				trim_start_and_end_inline(const_cast<char*>(ElementStr[0].c_str()));
			
				ClassAnalysis.ClassName = ElementStr[0];
				ClassAnalysis.CodeCPPName = ElementStr[0];
				//去除C和G前缀
				{
					char* ClearClassNamePtr = const_cast<char*>(ClassAnalysis.CodeCPPName.c_str());

					trim_start_and_end_inline(ClearClassNamePtr);

					// 移除头部C开头或者G开头
					remove_char_start(ClearClassNamePtr, 'C');
					remove_char_start(ClearClassNamePtr, 'G');
				}

				//考虑到多继承问题
				//public GObject ,public Interxx
				if (ElementStr.size() >= 2)
				{
					vector<std::string> InheritElement;

					simple_cpp_string_algorithm::parse_into_vector_array(
						ElementStr[1].c_str(), InheritElement, CommaString);

					for (auto& Tmp : InheritElement)
					{
						char* TmpPtr = const_cast<char*>(Tmp.c_str());
						trim_start_inline(TmpPtr);

						remove_string_start(TmpPtr, "public");
						remove_string_start(TmpPtr, "protected");
						remove_string_start(TmpPtr, "private");

						trim_start_inline(TmpPtr);

						ClassAnalysis.InheritName.push_back(Tmp);
					}
				}
			}
		
			//获取标记的成员函数
			if (Contain("CDIAPER"))
			{
				FFunctionAnalysis FunctionAnalysis;
				if (GetCodeTypeByFunc(Row, FunctionAnalysis))
				{
					Row = StringArray[i + 1];
					RowPtr = const_cast<char*>(Row.c_str());
					//static void Hello1(GObject *Context, int32 &A,float b,bool C);
				
					if (Contain("\tstatic") || Contain("static "))
					{
						FunctionAnalysis.bStatic = true;

						char R[1024] = { 0 };
						char L[1024] = { 0 };
						//remove_string_start();
						split(RowPtr, SpaceString, R, L, false);

						Row = L;
					}
					else if (Contain("virtual "))
					{
						FunctionAnalysis.bVirtual = true;

						char R[1024] = { 0 };
						char L[1024] = { 0 };

						split(RowPtr, SpaceString, R, L, false);

						Row = L;
					}

					//确定我们函数的返回类型
					char Tmp[1024] = { 0 };
					{
						//Row =  void Hello1(GObject *Context, int32 &A,float b,bool C);
					
						char R[1024] = { 0 };
						trim_start_inline(RowPtr);

						split(RowPtr, SpaceString, R, Tmp, false);

						////Tmp = Hello1(UObject *Context, int32 &A,float b,bool C);  {}
						FunctionAnalysis.Return = CollectionVariableType(R, ECollectionParmType::Type_Return);
						
						{
							//void Hello1(UObject *Context, int32 &A,float b,bool C
							remove_char_end(Tmp, '}');
							remove_char_end(Tmp, '{');
							trim_end_inline(Tmp);
							remove_char_end(Tmp, ';');
							remove_char_end(Tmp, ')');
							//Tmp = Hello1(UObject *Context, int32 &A,float b,bool C
						}

						char RStr[1024] = { 0 };
						char LStr[1024] = { 0 };

						split(Tmp, LeftParenthesisString, RStr, LStr, false);

						//函数名
						FunctionAnalysis.FunctionName = RStr;

						AnalyticParameters(LStr, FunctionAnalysis.ParamArray);

						ClassAnalysis.Function.push_back(FunctionAnalysis);
					}
				}
			}	

			////获取标记的成员变量
			if (Contain("CVARIABLE"))
			{
				if (Contain("CodeType"))
				{
					FVariableAnalysis VariableAnalysis;
					if (GetCodeTypeByProp(Row, &VariableAnalysis))
					{
						char R[1024] = { 0 };
						char L[1024] = { 0 };

						Row = StringArray[i + 1];
						RowPtr = const_cast<char*>(Row.c_str());

						//Row = \tTSubclassOf<UStaticMesh> Mesh;
						remove_char_start(RowPtr, '\t');
						remove_char_end(RowPtr, ';');

						//Row = TSubclassOf<UStaticMesh> Mesh
						if (Contain(StarString))
						{
							VariableAnalysis.bPointer = true;
							split(RowPtr, StarString, R, L, false);
						}

						if (Contain(FetchAddressString))
						{
							VariableAnalysis.bReference = true;
							split(RowPtr, FetchAddressString, R, L, false);
						}

						if (Contain(SpaceString))
						{
							split(RowPtr, SpaceString, R, L, false);
						}

						if (c_str_contain(R, "const"))
						{
							VariableAnalysis.bConst = true;
							remove_string_start(R, "const");
						}

						trim_start_and_end_inline(R);
						trim_start_and_end_inline(L);

						//是不是模板
						if (c_str_contain(R, "<") && c_str_contain(R, ">"))
						{
							//V<a,b,c,d,e,f,g,....>
							char TempR[1024] = { 0 };
							char TempL[1024] = { 0 };
							split(R, "<", TempR, TempL, false);

							VariableAnalysis.Type = TempR;

							//a,b,c,d,e,f,g,....>
							remove_char_end(TempL, '>');
							//a,b,c,d,e,f,g,....

							AnalyticParameters(TempL, VariableAnalysis.InternalType);
						}
						else
						{
							VariableAnalysis.Type = R;
						}
						
						VariableAnalysis.Name = L;

						ClassAnalysis.Variable.push_back(VariableAnalysis);
					}
				}
			}
		}

		return true;
	}
}