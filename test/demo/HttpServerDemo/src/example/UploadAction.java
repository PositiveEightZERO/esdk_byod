package example;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;

import org.apache.commons.io.FileUtils;
import org.apache.struts2.ServletActionContext;

import com.opensymphony.xwork2.ActionContext;

@SuppressWarnings("serial")
public class UploadAction extends ExampleSupport {
	private File image; // 上传的文件
	private String imageFileName; // 文件名称
	private String imageContentType; // 文件类型

	public String execute() throws Exception {
		String realpath = ServletActionContext.getServletContext().getRealPath("/images");
		System.out.println("realpath: " + realpath);
		if (image != null) {
			
			File savefile = new File(new File(realpath), imageFileName);
			if (!savefile.getParentFile().exists())
				savefile.getParentFile().mkdirs();
			FileUtils.copyFile(image, savefile);
			ActionContext.getContext().put("message", "文件上传成功");
		}
		return SUCCESS;
	}

	public File getImage() {
		return image;
	}

	public void setImage(File image) {
		this.image = image;
	}

	public String getImageFileName() {
		return imageFileName;
	}

	public void setImageFileName(String imageFileName) {
		
		try {
			imageFileName = URLDecoder.decode(imageFileName, "utf-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		System.out.println("----"+imageFileName);
		
		
		this.imageFileName = imageFileName;
	}

	public String getImageContentType() {
		return imageContentType;
	}

	public void setImageContentType(String imageContentType) {
		this.imageContentType = imageContentType;
	}

}
