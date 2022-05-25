package util;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.jcraft.jsch.Channel;
import com.jcraft.jsch.ChannelExec;
import com.jcraft.jsch.JSch;
import com.jcraft.jsch.JSchException;
import com.jcraft.jsch.Session;

public class SCPManager {

	public Session createSession(String user, String host, int port, String password) {
		try {
			JSch jsch = new JSch();

			Session session = jsch.getSession(user, host, port);

			return session;
		} catch (JSchException e) {
			System.out.println(e);
			return null;
		}
	}

	public void copyLocalToRemote(Session session, String from, String to, String fileName)
			throws JSchException, IOException {
		boolean ptimestamp = true;

		// exec 'scp -t rfile' remotely
		String command = "scp " + (ptimestamp ? "-p" : "") + " -t " + to;
		Channel channel = session.openChannel("exec");
		((ChannelExec) channel).setCommand(command);

		// get I/O streams for remote scp
		OutputStream out = channel.getOutputStream();
		InputStream in = channel.getInputStream();

		channel.connect();

		if (checkAck(in) != 0) {
			System.exit(0);
		}

		File _lfile = new File(from);

		if (ptimestamp) {
			command = "T" + (_lfile.lastModified() / 1000) + " 0";
			// The access time should be sent here,
			// but it is not accessible with JavaAPI ;-<
			command += (" " + (_lfile.lastModified() / 1000) + " 0\n");
			out.write(command.getBytes());
			out.flush();
			if (checkAck(in) != 0) {
				System.exit(0);
			}
		}

		// send "C0644 filesize filename", where filename should not include '/'
		long filesize = _lfile.length();
		command = "C0644 " + filesize + " ";
		if (from.lastIndexOf('/') > 0) {
			command += fileName.substring(from.lastIndexOf('/') + 1);
		} else {
			command += fileName;
		}

		command += "\n";
		out.write(command.getBytes());
		out.flush();

		if (checkAck(in) != 0) {
			System.exit(0);
		}

		// send a content of lfile
		FileInputStream fis = new FileInputStream(from);
		byte[] buf = new byte[1024];
		while (true) {
			int len = fis.read(buf, 0, buf.length);
			if (len <= 0)
				break;
			out.write(buf, 0, len); // out.flush();
		}

		// send '\0'
		buf[0] = 0;
		out.write(buf, 0, 1);
		out.flush();

		if (checkAck(in) != 0) {
			System.exit(0);
		}
		out.close();

		try {
			if (fis != null)
				fis.close();
		} catch (Exception ex) {
			System.out.println(ex);
		}

		channel.disconnect();
		session.disconnect();
	}

	public int checkAck(InputStream in) throws IOException {
		int b = in.read();
		// b may be 0 for success,
		// 1 for error,
		// 2 for fatal error,
		// -1
		if (b == 0)
			return b;
		if (b == -1)
			return b;

		if (b == 1 || b == 2) {
			StringBuffer sb = new StringBuffer();
			int c;
			do {
				c = in.read();
				sb.append((char) c);
			} while (c != '\n');
			if (b == 1) { // error
				System.out.print(sb.toString());
			}
			if (b == 2) { // fatal error
				System.out.print(sb.toString());
			}
		}
		return b;
	}
}
