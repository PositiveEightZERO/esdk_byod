package example;

import java.io.File;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.nio.charset.Charset;

import javax.servlet.http.HttpServletResponse;

import org.apache.struts2.ServletActionContext;

@SuppressWarnings("serial")
public class DownloadAction extends ExampleSupport {

	private String fileName;

	// public void SUCCESS(String fileName) {
	// this.fileName = fileName;
	// }

	public void setFileName(String fileName) {
		try {
			fileName = new String(fileName.getBytes("ISO-8859-1"), "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		// System.out.println("----"+fileName);
		this.fileName = fileName;
	}

	public String getDownloadFileName() {
		String downFileName = fileName;
		// System.out.println("----"+downFileName);
		try {
			downFileName = new String(downFileName.getBytes(), "ISO8859-1");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		// System.out.println("----"+downFileName);
		return downFileName;

	}

	public InputStream getInputStream() {
		System.out.println("fileName:" + fileName);

		if (fileName != null) {

			// fileName = new String(fileName.getBytes("ISO-8859-1"), "UTF-8");

			// System.out.println("fileName UTF-8:" + fileName);

			HttpServletResponse response = ServletActionContext.getResponse();
			response.setHeader("Content-Disposition", "attachment;fileName="
					+ getDownloadFileName());

			String realpath = ServletActionContext.getServletContext()
					.getRealPath("/images/");
			File file = new File(new File(realpath), fileName);

			System.out.print(realpath);

			if (file.exists()) {

				return ServletActionContext.getServletContext()
						.getResourceAsStream("/images/" + fileName);
			} else {
				response.setStatus(404);
				return null;
			}

		} else {

			return null;
		}

	}

	public String execute() {
		return SUCCESS;
	}
}
