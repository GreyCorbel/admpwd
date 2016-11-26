using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AdmPwd.Utils;
using AdmPwd.PSTypes;

namespace AdmPwd.UI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private string computerDN=null;

        private void btnSearch_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(txtComputerName.Text.Trim()))
            {
                lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("MustSpecifyComputerName");
                return;
            }
            lblStatus.Text = string.Empty;
            string compDN;

            if (txtComputerName.Text.StartsWith("CN=", StringComparison.OrdinalIgnoreCase))
            {
                compDN = txtComputerName.Text;
            }
            else
            {
                var DNs=DirectoryUtils.GetComputerDN(txtComputerName.Text);
                if(DNs.Count==0) {
                    lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("ComputerNotFound"); 
                    return;
                }
                if(DNs.Count>1) {
                    lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("ComputerNameAmbiguous");
                    return;
                }
                compDN = DNs[0];
            }
            PasswordInfo pi = DirectoryUtils.GetPasswordInfo(compDN);
            if (pi == null)
            {
                lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("ComputerNotFound");
                return;
            }
            this.txtPassword.Text = pi.Password;
            this.txtCurrentPwdExpiration.Text = pi.ExpirationTimestamp.ToString();
            this.computerDN = pi.DistinguishedName;
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnForceExpiration_Click(object sender, EventArgs e)
        {
            lblStatus.Text = string.Empty;
            if (this.computerDN == null)
            {
                lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("NoComputerSelected");
                    return;
            }
            DateTime newPwdExpiration = DateTime.Now;
            if (!string.IsNullOrEmpty(txtPwdExpiration.Text.Trim()))
            {
                try
                {
                    newPwdExpiration = System.Convert.ToDateTime(txtPwdExpiration.Text.Trim(),System.Globalization.DateTimeFormatInfo.CurrentInfo);
                }
                catch (FormatException)
                {
                    lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("InvalidDateFormat");
                    return;
                }
            }
            try {
                DirectoryUtils.ResetPassword(this.computerDN,newPwdExpiration);
                //read back from AD
                PasswordInfo pi = DirectoryUtils.GetPasswordInfo(computerDN);
                if (pi == null)
                {
                    lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("ComputerNotFound");
                    return;
                }
                this.txtPassword.Text = pi.Password;
                this.txtCurrentPwdExpiration.Text = pi.ExpirationTimestamp.ToString();
                this.computerDN = pi.DistinguishedName;
            }
            catch(Exception) {
                lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("PasswordResetFailed");
                return;
            }
            lblStatus.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("PasswordResetSucceeded");
        }
    }
}
