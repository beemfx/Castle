
package webcastle;
import java.net.*;

public class CastleGameJ 
{
	//Definitions:
	//Function types:
	private static final int PRINT=0x00000001;
	private static final int GOTO=0x00000002;
	private static final int CHOICE=0x00000003;
	private static final int CLS=0x00000004;
	private static final int END=0x00000005;
	private static final int COMMENT=0x00000006;
	private static final int FUNCTIONFAIL=0x10000000;
	
	//Statement types
	private static final int ST_FUNCTION=0x00000000;
	private static final int ST_LABEL=0x00000001;
	private static final int ST_FAIL=0x10000000;
	private static final int ST_UNKNOWN=0x10000001;
	
	//Various constants:
	private static final int MAX_PATH=260;
	private static final int MAX_LINES=40;
	private static final int MAX_CHARS_PER_LINE=200;
	private static final int MAX_PARAMS=10;
	public static final int MAX_CHOICES=10;
	
	
	//Private:
	private DataStreamJ m_pFile;
	
	int m_nCurrentLine=0; //The line to print on
	int m_nNumChoices=0;
	
	int m_nMapVersion=0;  //The current map's version
	char m_cMapEdition='A';  //The current map's edition
	char m_szMapName[]=new char[MAX_PATH]; //The name of the map default map is WINCASTLE
	
	private char[] m_szTemp=new char[MAX_CHARS_PER_LINE];
	
	char m_lpOutputData[][]=new char[MAX_LINES][MAX_CHARS_PER_LINE];
	char m_lpGotoChoice[][]=new char[MAX_CHOICES][MAX_CHARS_PER_LINE];
	

	public CastleGameJ(String szInitialMap) 
	{
		LoadMap(szInitialMap);
	}
	
	
	public CastleGameJ(URL urlInitialMap)
	{
		LoadMap(urlInitialMap);
	}
	
	boolean LoadMap(URL urlInitialMap)
	{
		m_pFile=new DataStreamJ();
		if(!m_pFile.Open(urlInitialMap))
			return false;
		
		m_lpOutputData[0][0]=0;
		m_nCurrentLine=0;
		m_nNumChoices=0;
		Initialize();
		return true;
	}
	
	boolean LoadMap(String szFilename)
	{
		//Open the file, return false if the file can't be opened.
		m_pFile=new DataStreamJ();
		if(!m_pFile.Open(szFilename))
			return false;

		m_lpOutputData[0][0]=0;
		m_nCurrentLine=0;
		m_nNumChoices=0;
		Initialize();
		return true;
	}
	
	private boolean Initialize()
	{
		//SetDataStreamPointer(m_hDataStream, 0, STREAM_BEGIN);
		m_pFile.Seek(0, DataStreamJ.MOVE_START);

		//We initialize by reading the:
		//	map name
		ReadStatement(m_szMapName, MAX_CHARS_PER_LINE);
		//	map version
		ReadStatement(m_szTemp, MAX_CHARS_PER_LINE);
		m_nMapVersion=AtoI(m_szTemp);
		//Map Edition
		ReadStatement(m_szTemp, MAX_CHARS_PER_LINE);
		m_cMapEdition=m_szTemp[0];

		ProcessGameUntilBreak();
		return true;
	}

	int GetMapVersion()
	{
		return m_nMapVersion;
	}

	char GetMapEdition()
	{
		return m_cMapEdition;
	}

	String GetMapName()
	{
		return new String(m_szMapName, 0, StrLen(m_szMapName));
	}
	
	public int GetNumChoices()
	{
		return m_nNumChoices;
	}
	
	public int GetNumOutputLines()
	{
		return m_nCurrentLine;
	}
	
	public String GetOutputLine(int i)
	{
		return new String(m_lpOutputData[i], 0, StrLen(m_lpOutputData[i]));
	}
	
	private int ReadStatement(char szOut[], int nMaxLen)
	{
		boolean bInsideQuotes=false;
	
		byte pChar[]=new byte[1];
		//byte sChar=0;
		//sChar[0]=0;
		
		
		int i=0;
		while(true)
		{
			//Read one character
			
			if((m_pFile.Read(pChar, 1)==0) || i>=nMaxLen)
				return ST_FAIL; 
			//sChar=pChar[0];
			
			if(pChar[0]==' ' && !bInsideQuotes)
				;
			//This line will remove tabs newlines null characters and return carriages
			else if(pChar[0]==('\n') || pChar[0]==('\t') || pChar[0]==('\0') || pChar[0]==('\r'))
				;
			else if( (pChar[0]==(';')) && (!bInsideQuotes) )
			{
				szOut[i]=0;
				return ST_FUNCTION;
			}
			else if( (pChar[0]==(':')) && (!bInsideQuotes) )
			{
				szOut[i]=0;
				return ST_LABEL;
			}
			else if(pChar[0]==('"'))
			{
				if(bInsideQuotes)bInsideQuotes=false;
				else bInsideQuotes=true;
				szOut[i]=('"');
				i++;
			}
			else
			{
				szOut[i]=(char)pChar[0];
				i++;
			}
		}
		
		//return ST_UNKNOWN;
	}

	private void GetString(char[] szStringOut, char[] szLine)
	{
		int nLength = (int)StrLen(szLine);

		int j=0;
		boolean bActive=false;
		for(int i=0; i<nLength; i++){
			if(!bActive){
				if(szLine[i]==('"'))
					bActive=true;
			}
			else if(bActive){
				if(szLine[i]==('"')){szStringOut[j]=0;break;}
				szStringOut[j]=szLine[i];
				j++;
			}
		}
	}
	
	private boolean Seek(char[] szLabel)
	{
		//we need to find the line that matches the label

		//First we go to the beginning of the file
		//REWIND(m_hInputFile);
		//SetDataStreamPointer(m_hDataStream, 0, STREAM_BEGIN);
		m_pFile.Seek(0, DataStreamJ.MOVE_START);

		char szString[]=m_szTemp;

		while(true)
		{
			switch(ReadStatement(szString, MAX_CHARS_PER_LINE))
			{
				case ST_LABEL:
					if(StrnCmp(szString, szLabel, MAX_CHARS_PER_LINE))return true;
					break;
				case ST_FAIL:
					//if st_fail then we probably hit the end of the file
					return false;
				default:
					//loop around again
					break;
			}
			
			if(m_pFile.Tell()==m_pFile.GetSize())
				break;
		}
		return false;
	}

	private int GetParams(char[] szLine, char pszParams[][])
	{
		int nParam=0;
		boolean bGettingParams=false;
		boolean bGettingString=false;
		int j=0;
		for(int i=0; i<(int)StrLen(szLine); i++)
		{
			if(!bGettingString){
				if(szLine[i]==('(')){
					bGettingParams=true;
					continue;
				}

				if(szLine[i]==(')')){
					pszParams[nParam][j]=0;
					break;
					}
			}

			if(bGettingParams)
			{
				if(szLine[i]==(','))
				{
					pszParams[nParam][j]=0;
					j=0;
					nParam++;
				}
				else if(szLine[i]==('"'))
				{
					pszParams[nParam][j]=szLine[i];
					if(bGettingString)bGettingString=false;
					else bGettingString=true;
					j++;
				}
				else
				{
					pszParams[nParam][j]=szLine[i]; 
					j++;
				}
			}
		}
		//return the number of Parameters gotten
		return nParam+1;
	}
	
	private int GetFunction(char[] szLine)
	{
		//We have the line, lets determine the function and
		//remove the function from the line
		char szFunction[]=m_szTemp;


		//We get which function name by reading the line until a '(' is encoutnered
		int j=0;
		for(int i=0; i<(int)StrLen(szLine); i++){
			if(szLine[i]==('(')){
				szFunction[j]=0;
				break;
			}else{
				szFunction[j]=szLine[i];
				j++;
			}
		}
		
		String szFunc;

		//We now have the function name lets return it
		int nLen=StrLen(szFunction);
		
		if(StrnCmp(szFunction, "CLS", nLen))return CLS;
		if(StrnCmp(szFunction, "PRINT", nLen))return PRINT;
		if(StrnCmp(szFunction, "CHOICE", nLen))return CHOICE;
		if(StrnCmp(szFunction, "GOTO", nLen))return GOTO;
		if(StrnCmp(szFunction, "END", nLen))return END;

		//if the function was not valid we return function fail
		return FUNCTIONFAIL;
	}
	
	public void Restart()
	{
		m_pFile.Seek(0, DataStreamJ.MOVE_START);
		m_nCurrentLine=0;
		m_lpOutputData[0][0]=0;
		Initialize();
	}
	
	boolean SendInput(int nChoice)
	{
		nChoice--;
		if(nChoice<0 || nChoice>=m_nNumChoices)return false;
		Seek(m_lpGotoChoice[nChoice]);
		m_nNumChoices=0;
		ProcessGameUntilBreak();
		return true;
	}
	
	
	private void CompileError(char[] szError)
	{
		//Error procedure.
	}
	
	private void CompileError(String szError)
	{
		CompileError(szError.toCharArray());
	}

	private boolean ProcessFunction(int fnResult, char[] szFunctionStatement)
	{
		switch(fnResult)
		{
		case END:
			//Print game over and break
			//StrCpy(m_lpOutputData[m_nCurrentLine], ("Game Over"));
			//m_lpOutputData[m_nCurrentLine]="Game Over";
			StrCpy(m_lpOutputData[m_nCurrentLine], "Game Over");
			m_nNumChoices=0;
			m_nCurrentLine++;
			break;
		case PRINT:
			//For a print we need to get the string to print, this is as simple
			//as calling the GetString Fucntion
			GetString(szFunctionStatement, szFunctionStatement);
			StrCpy(m_lpOutputData[m_nCurrentLine], szFunctionStatement);
			m_nCurrentLine++;
			break;
		case CHOICE:
			//For a choice we need to print out the string, and store the other perameters
			//
			//MessageBox(0, szCurrentString, 0, 0);
			char szTempString[][]=new char[MAX_CHOICES][MAX_CHARS_PER_LINE];
			int nNumParams;
			nNumParams=GetParams(szFunctionStatement, szTempString);
			GetString(szTempString[0], szTempString[0]);
			//we subtract 1 from the number of choices
			//because the first parameter is not a choice
			m_nNumChoices=nNumParams-1;
			for(int i=0; i<m_nNumChoices; i++)
			{
				StrCpy(m_lpGotoChoice[i], szTempString[i+1]);
			}
			StrCpy(m_lpOutputData[m_nCurrentLine], szTempString[0]);

			m_nCurrentLine++;
			break;
		case COMMENT://Do nothing
			break;
		case GOTO:
			char szParams[][]=new char[1][MAX_CHARS_PER_LINE];
			GetParams(szFunctionStatement, szParams);
			Seek(szParams[0]);
			break;
		case CLS:
			for(int i=0; i<MAX_LINES; i++)
				m_lpOutputData[i][0]=0;
			m_nCurrentLine=0;
			break;
		default:
			return false;
		}
		
		return true;
	}
	
	private boolean ProcessGameUntilBreak()
	{
		char[] szCurrentString=new char[MAX_CHARS_PER_LINE];
		int eFunction=FUNCTIONFAIL;

		int nNumLoops=0;

		while(true)
		{
			//Read the next statement
			int stResult=ReadStatement(szCurrentString, MAX_CHARS_PER_LINE);
			if(stResult == ST_FUNCTION)
			{
				eFunction=GetFunction(szCurrentString);
				ProcessFunction(eFunction, szCurrentString);
				if(eFunction==END||eFunction==CHOICE)
					break;
			}
			else if(stResult==ST_LABEL)
			{
				//we don't need to do anything on a label
			}
			else
			{
				CompileError(("Error: An error occured while translating the script!"));
			}

			//if this loop loops too many times we quit because there is probably
			//a problem with the script (for example an infinite GOTO loop)
			nNumLoops++;
			if(nNumLoops>1000){CompileError(("Error: Possible Infinite Loop!"));return false;}
		}
		return true;
	}

 //Private:
	private int StrLen(char[] szString)
	{
		int nCount=0;
		while(szString[nCount]!=0 && nCount < szString.length)
			nCount++;
		
		return nCount;
	}
	
	private void StrCpy(char[]szOut, String szIn)
	{
		int i=0;
		for(i=0; i<szIn.length(); i++)
		{
			szOut[i]=szIn.charAt(i);
		}
		szOut[i]=0;
	}
	
	private void StrCpy(char[]szOut, char[]szIn)
	{
		for(int i=0; ;i++)
		{
			szOut[i]=szIn[i];
			if(szOut[i]==0)
				break;
		}
	}
	
	private void AnsiToUnicode(char[] szOut, byte[]szIn)
	{
		for(int i=0; ;i++)
		{
			szOut[i]=(char)szIn[i];
			if(szOut[i]==0)
				break;
		}
	}
	
	private boolean StrnCmp(char[]szOne, String szTwo, int nNum)
	{
		int i=0;
		int nLen1=0, nLen2=0;
		char c1=0, c2=0;

		nLen1=StrLen(szOne);
		nLen2=szTwo.length();

		if(nNum>0)
		{
			if(nLen1>nNum)
				nLen1=nNum;
			if(nLen2>nNum)
				nLen2=nNum;
		}

		if(nLen1!=nLen2)
			return false;

		for(i=0; i<nLen1; i++)
		{
			if(szOne[i]!=szTwo.charAt(i))
			{
				c1=szOne[i];
				c2=szTwo.charAt(i);

				if( (c1>='a') && (c1<='z') )
					c1 -= ('a'-'A');
				if( (c2>='a') && (c2<='z') )
					c2 -= ('a'-'A');
				if(c1==c2)
					continue;

				return false;
			}
		}
		return true;
	}
	
	private boolean StrnCmp(char[] szOne, char[] szTwo, int nNum)
	{
		int i=0;
		int nLen1=0, nLen2=0;
		char c1=0, c2=0;

		nLen1=StrLen(szOne);
		nLen2=StrLen(szTwo);

		if(nNum>0)
		{
			if(nLen1>nNum)
				nLen1=nNum;
			if(nLen2>nNum)
				nLen2=nNum;
		}

		if(nLen1!=nLen2)
			return false;

		for(i=0; i<nLen1; i++)
		{
			if(szOne[i]!=szTwo[i])
			{
				c1=szOne[i];
				c2=szTwo[i];

				if( (c1>='a') && (c1<='z') )
					c1 -= ('a'-'A');
				if( (c2>='a') && (c2<='z') )
					c2 -= ('a'-'A');
				if(c1==c2)
					continue;

				return false;
			}
		}
		return true;
	}
 
	public static int AtoI(char[]string)
	{
		int lValue=0;
		char c=0;
		int i=0;
		int sign=1;

		sign=(string[0]=='-')?-1:1;

		for(i=(sign>0)?0:1, lValue=0; ;i++)
		{
			c=string[i];
			if(c<'0' || c>'9')
				break;

			lValue=lValue*10+c-'0';
		}

		return sign*lValue;
	}
}

