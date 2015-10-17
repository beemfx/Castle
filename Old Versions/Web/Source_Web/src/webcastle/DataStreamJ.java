package webcastle;

import java.io.*;
import java.net.*;

public class DataStreamJ 
{
	//Private:
	private boolean m_bOpen=false;
	private byte m_pData[];
	private int m_nDataPointer=0;
	private int m_nSize=0;
	
	//Definitions:
	public static final int MOVE_START=0;
	public static final int MOVE_CUR=1;
	public static final int MOVE_END=2;
	
	//Public:
	public DataStreamJ() 
	{
	}
	
	public DataStreamJ(String szFile)
	{
		Open(szFile);
	}
	
	boolean Open(java.net.URL urlFile)
	{
		Close();
		
		try
		{
			/*
			File fin=new File(urlFile);
			FileInputStream file=new FileInputStream(fin);
			 */
			URLConnection urlc=urlFile.openConnection();
			InputStream file=urlc.getInputStream();
			
			m_nSize=urlc.getContentLength();
			if(m_nSize==0)
			{
				file.close();
				return false;
			}
			
			m_pData=new byte[m_nSize];
			
			if(m_pData==null)
			{
				file.close();
				return false;
			}
			int nBytesRead=0;
			
	
			do
			{
				int nTemp=file.read(m_pData, nBytesRead, m_nSize-nBytesRead);
				if(nTemp>=0)
					nBytesRead+=nTemp;
				else
					break;
			}while(true);
			
			m_nDataPointer=0;
			m_bOpen=true;
		
			file.close();	
		}
		catch (IOException e)
		{
			return false;
		}
		
		return true;
	}

	boolean Open(String szFile)
	{
		//Close the file if it happens to be open.
		Close();
		
		try
		{
			File fin=new File(szFile);
			FileInputStream file=new FileInputStream(fin);
			m_nSize=(int)fin.length();
			
			if(m_nSize==0)
				return false;
			
			m_pData=new byte[m_nSize];
			if(m_pData==null)
			{
				file.close();
				m_nSize=0;
				return false;
			}
			
			//Read the file.
			file.read(m_pData);
	
			file.close();
			m_nDataPointer=0;
			m_bOpen=true;	
		}
		catch(IOException e)
		{
			Close();
			return false;
		}
		
		return true;
	}
	
	void Close()
	{
		m_nSize=0;
		m_nDataPointer=0;
		m_pData=null;
		m_bOpen=false;
	}
	
	int Read(byte[] buffer, int count)
	{
		if(!m_bOpen)
			return 0;
		if(m_nSize < (m_nDataPointer + count))
			count=m_nSize-m_nDataPointer;

		for(int i=0; i<count; i++)
		{
			buffer[i]=m_pData[m_nDataPointer+i];
		}
		m_nDataPointer+=count;
		return count;
	}
	
	int GetSize()
	{
		if(!m_bOpen)
			return 0;
		return m_nSize;
	}
	
	int Seek(int nDistance, int nMethod)
	{
		if(!m_bOpen)
			return 0;
		
		switch(nMethod)
		{
		case MOVE_START:
			m_nDataPointer=nDistance;
			break;
		case MOVE_CUR:
			m_nDataPointer+=nDistance;
			break;
		case MOVE_END:
			m_nDataPointer=m_nSize+nDistance;
			break;
		}
		
		//Make sure the new data pointer is within the file data.
		m_nDataPointer=m_nDataPointer>m_nSize?m_nSize:m_nDataPointer<0?0:m_nDataPointer;
	
		return m_nDataPointer;
	}
	
	int Tell()
	{
		if(!m_bOpen)
			return 0;
		
		return m_nDataPointer;
	}
}
