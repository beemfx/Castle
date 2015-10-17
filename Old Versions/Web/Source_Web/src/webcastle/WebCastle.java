
package webcastle;

import java.awt.*;
import webcastle.*;
import javax.swing.*;
import java.awt.event.*;

public class WebCastle extends javax.swing.JApplet implements KeyListener, ActionListener
{
	private CastleGameJ m_cGame;
	private JButton m_pButtons[];/*={
		new JButton("Choice 1"),
		new JButton("Choice 2"),
		new JButton("Choice 3"),
		new JButton("Choice 4"),
		new JButton("Choice 5"),
		new JButton("Choice;
		*/
	
	public void init()
	{
		setLayout(null);
		addKeyListener(this);
		m_pButtons=new JButton[CastleGameJ.MAX_CHOICES+1];
		for(int i=0; i<CastleGameJ.MAX_CHOICES; i++)
		{
			m_pButtons[i]=new JButton("Choice " + (i+1));
			m_pButtons[i].setBounds(0, 0, 110, 30);
			m_pButtons[i].setLocation(i*115+10, this.getHeight()-40);
			m_pButtons[i].addActionListener(this);
			add(m_pButtons[i]);
		}
		m_pButtons[CastleGameJ.MAX_CHOICES]=new JButton("Restart");
		m_pButtons[CastleGameJ.MAX_CHOICES].setBounds(0, 0, 110, 30);
		m_pButtons[CastleGameJ.MAX_CHOICES].setLocation(this.getWidth()-120, this.getHeight()-40);
		m_pButtons[CastleGameJ.MAX_CHOICES].addActionListener(this);
		add(m_pButtons[CastleGameJ.MAX_CHOICES]);
		try
		{
		java.net.URL urlFile=new java.net.URL(getCodeBase().toString()+getParameter("map"));
		m_cGame=new CastleGameJ(urlFile);
		}
		catch(java.net.MalformedURLException e)
		{
			m_cGame=null;
		}
		HideChoices();
	}
	
	private void HideChoices()
	{
		if(m_cGame==null)
			return;
		
		for(int i=0; i<CastleGameJ.MAX_CHOICES; i++)
		{
			if(i<m_cGame.GetNumChoices())
				m_pButtons[i].setVisible(true);
			else
				m_pButtons[i].setVisible(false);
		}
	}
	
	public void ProcessInput(int c)
	{
		if(c==0)
			m_cGame.Restart();
		else
			m_cGame.SendInput((char)c);
		
		HideChoices();
		repaint();
	}
	
	public void messageBox(String szMessage)
	{
		JOptionPane.showMessageDialog(this, szMessage);
	}
	public void actionPerformed(ActionEvent e)
	{
		if(e.getSource()==m_pButtons[CastleGameJ.MAX_CHOICES])
		{
			ProcessInput(0);
		}
		else
		{
			for(int i=0; i<CastleGameJ.MAX_CHOICES; i++)
			{
				if(e.getSource()==m_pButtons[i])
				{
					ProcessInput(i+1);
					break;
				}
			}
		}
		//validate();
		
	}
	
	public void keyReleased(KeyEvent txt)
	{
	}
	public void keyPressed(KeyEvent txt)
	{
	}
	public void keyTyped(KeyEvent input)
	{
		char c=input.getKeyChar();
		if(c>=48 && c<=57)
			m_cGame.SendInput(c-'0');
		else if(c=='r' || c=='R')
			m_cGame.Restart();
		
		this.repaint();
	}
	
	public void paint(Graphics screen)
	{
		if(m_cGame==null)
			return;
		
		super.paint(screen);
		
		
		Graphics2D screen2D=(Graphics2D)screen;
		screen2D.setFont(new Font("Verdana", Font.PLAIN, 14));
		//screen.clearRect(0, 0, 500, 320);
		for(int i=0; i<m_cGame.GetNumOutputLines(); i++)
		{
			screen2D.drawString(m_cGame.GetOutputLine(i), 0, screen2D.getFont().getSize()*(i+1));	
		}
	}
}
