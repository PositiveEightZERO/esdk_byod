package example;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.ServletActionContext;

public class Userinfo extends ExampleSupport 
{
	public String execute() throws Exception {

        HttpServletRequest request = ServletActionContext.getRequest ();
        String username ;
        String password ;
        
        
        try
        {
        
        	username = request.getSession().getAttribute("username").toString();
        	password = request.getSession().getAttribute("password").toString();
        
        }
        catch(Exception ex)
        {
        	username = "";
        	password = "";
        }
        if(username == "")
        {
        	userinfo = "No login";
        }
        else
        {
        	userinfo = "login,username:" + username + ",password:" + password;
        }
      
        return SUCCESS;
    }

	 private String userinfo;

    public String getUserinfo() {
		return userinfo;
	}

	public void setUserinfo(String userinfo) {
		this.userinfo = userinfo;
	}

}
