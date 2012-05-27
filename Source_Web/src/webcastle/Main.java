/*
 * Main.java
 *
 * Created on December 9, 2006, 7:52 PM
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package webcastle;

import java.io.*;

public class Main
{

	CastleGameJ m_cGame;
		
	private char GetCh()
	{
		try
		{
		return (char)System.in.read();
		}
		catch(IOException e)
		{
		return 0;
		}
	}
	private void ClrCls()
	{
		for(int i=0; i<80; i++)
			System.out.println("");
	}
	private void DisplayOutput()
	{
		ClrCls();
		for(int i=0; i<m_cGame.GetNumOutputLines(); i++)
		{
			System.out.printf("%s\n", m_cGame.GetOutputLine(i));
		}
	}
	
	public Main(String szMap)
	{
		m_cGame=new CastleGameJ(szMap);
		
		System.out.printf("Running %s (%c), using BUILD %d\n", 
			m_cGame.GetMapName(), 
			m_cGame.GetMapEdition(), 
			m_cGame.GetMapVersion());
		
		do
		{
			
			//m_cGame.SendInput(1);
			//GetCh();
			DisplayOutput();
			
			char c=GetCh();
			if(c==27 || c=='x' || c== 'X')
				break;
			else if(c=='r')
				m_cGame.Restart();//m_cGame.Restart();
			else if(c=='o' || c=='O')
				;
			
			m_cGame.SendInput(c-48);
			
			
		}while(true);
		
		
	}
	
	public static void main(String[] args)
	{
		Main program=new Main("Adventure.tba");
	}
}
