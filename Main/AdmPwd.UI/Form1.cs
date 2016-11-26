using System;
using System.Windows.Forms;
using AdmPwd.Utils;
using AdmPwd.PSTypes;
using AdmPwd.UI.Properties;

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
            try
            {
                if (string.IsNullOrEmpty(txtComputerName.Text.Trim()))
            {
                lblStatus.Text = Resources.MustSpecifyComputerName;
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
                    lblStatus.Text = Resources.ComputerNotFound; 
                    return;
                }
                if(DNs.Count>1) {
                    lblStatus.Text = Resources.ComputerNameAmbiguous;
                    return;
                }
                compDN = DNs[0];
            }
            PasswordInfo pi = DirectoryUtils.GetPasswordInfo(compDN);
            if (pi == null)
            {
                lblStatus.Text = Resources.ComputerNotFound;
                return;
            }
            this.txtPassword.Text = pi.Password;
            this.txtCurrentPwdExpiration.Text = pi.ExpirationTimestamp.ToString();
            this.computerDN = pi.DistinguishedName;
            }
            catch (Exception ex)
            {
                lblStatus.Text = ex.Message;
                txtPassword.Text = string.Empty;
                txtCurrentPwdExpiration.Text = string.Empty;
            }
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
                lblStatus.Text = Resources.NoComputerSelected;
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
                    lblStatus.Text = Resources.InvalidDateFormat;
                    return;
                }
            }
            try {
                DirectoryUtils.ResetPassword(this.computerDN,newPwdExpiration);
                //read back from AD
                PasswordInfo pi = DirectoryUtils.GetPasswordInfo(computerDN);
                if (pi == null)
                {
                    lblStatus.Text = Resources.ComputerNotFound;
                    return;
                }
                this.txtPassword.Text = pi.Password;
                this.txtCurrentPwdExpiration.Text = pi.ExpirationTimestamp.ToString();
                this.computerDN = pi.DistinguishedName;
            }
            catch(Exception) {
                lblStatus.Text = Resources.PasswordResetFailed;
                return;
            }
            lblStatus.Text = Resources.PasswordResetSucceeded;
        }
    }
}
