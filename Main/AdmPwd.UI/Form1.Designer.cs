namespace AdmPwd.UI
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.txtPwdExpiration = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSearch = new System.Windows.Forms.Button();
            this.txtComputerName = new System.Windows.Forms.TextBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.txtCurrentPwdExpiration = new System.Windows.Forms.TextBox();
            this.lblCurrentPwdExpiration = new System.Windows.Forms.Label();
            this.btnForceExpiration = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtPassword = new System.Windows.Forms.TextBox();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            this.btnExit = new System.Windows.Forms.Button();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtPwdExpiration
            // 
            resources.ApplyResources(this.txtPwdExpiration, "txtPwdExpiration");
            this.txtPwdExpiration.Name = "txtPwdExpiration";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.btnSearch);
            this.panel1.Controls.Add(this.txtComputerName);
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.Name = "panel1";
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // btnSearch
            // 
            resources.ApplyResources(this.btnSearch, "btnSearch");
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.UseVisualStyleBackColor = true;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // txtComputerName
            // 
            resources.ApplyResources(this.txtComputerName, "txtComputerName");
            this.txtComputerName.Name = "txtComputerName";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.txtCurrentPwdExpiration);
            this.panel2.Controls.Add(this.lblCurrentPwdExpiration);
            this.panel2.Controls.Add(this.btnForceExpiration);
            this.panel2.Controls.Add(this.label3);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.txtPassword);
            this.panel2.Controls.Add(this.txtPwdExpiration);
            resources.ApplyResources(this.panel2, "panel2");
            this.panel2.Name = "panel2";
            // 
            // txtCurrentPwdExpiration
            // 
            resources.ApplyResources(this.txtCurrentPwdExpiration, "txtCurrentPwdExpiration");
            this.txtCurrentPwdExpiration.Name = "txtCurrentPwdExpiration";
            this.txtCurrentPwdExpiration.ReadOnly = true;
            // 
            // lblCurrentPwdExpiration
            // 
            resources.ApplyResources(this.lblCurrentPwdExpiration, "lblCurrentPwdExpiration");
            this.lblCurrentPwdExpiration.Name = "lblCurrentPwdExpiration";
            // 
            // btnForceExpiration
            // 
            resources.ApplyResources(this.btnForceExpiration, "btnForceExpiration");
            this.btnForceExpiration.Name = "btnForceExpiration";
            this.btnForceExpiration.UseVisualStyleBackColor = true;
            this.btnForceExpiration.Click += new System.EventHandler(this.btnForceExpiration_Click);
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // txtPassword
            // 
            resources.ApplyResources(this.txtPassword, "txtPassword");
            this.txtPassword.Name = "txtPassword";
            this.txtPassword.ReadOnly = true;
            this.txtPassword.TabStop = false;
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // btnExit
            // 
            resources.ApplyResources(this.btnExit, "btnExit");
            this.btnExit.Name = "btnExit";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblStatus});
            resources.ApplyResources(this.statusStrip, "statusStrip");
            this.statusStrip.Name = "statusStrip";
            // 
            // lblStatus
            // 
            this.lblStatus.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.lblStatus.Name = "lblStatus";
            resources.ApplyResources(this.lblStatus, "lblStatus");
            // 
            // Form1
            // 
            this.AcceptButton = this.btnSearch;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Name = "Form1";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtPwdExpiration;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSearch;
        private System.Windows.Forms.TextBox txtComputerName;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtPassword;
        private System.Windows.Forms.ErrorProvider errorProvider1;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.Button btnForceExpiration;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel lblStatus;
        private System.Windows.Forms.TextBox txtCurrentPwdExpiration;
        private System.Windows.Forms.Label lblCurrentPwdExpiration;
    }
}

