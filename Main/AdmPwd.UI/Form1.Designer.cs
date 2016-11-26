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
            this.txtPwdExpiration.Location = new System.Drawing.Point(3, 122);
            this.txtPwdExpiration.Name = "txtPwdExpiration";
            this.txtPwdExpiration.Size = new System.Drawing.Size(282, 20);
            this.txtPwdExpiration.TabIndex = 3;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.btnSearch);
            this.panel1.Controls.Add(this.txtComputerName);
            this.panel1.Location = new System.Drawing.Point(13, 13);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(354, 52);
            this.panel1.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 4);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("ComputerName");
            // 
            // btnSearch
            // 
            this.btnSearch.Location = new System.Drawing.Point(291, 22);
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.Size = new System.Drawing.Size(63, 23);
            this.btnSearch.TabIndex = 2;
            this.btnSearch.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("Search");
            this.btnSearch.UseVisualStyleBackColor = true;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // txtComputerName
            // 
            this.txtComputerName.Location = new System.Drawing.Point(3, 22);
            this.txtComputerName.Name = "txtComputerName";
            this.txtComputerName.Size = new System.Drawing.Size(282, 20);
            this.txtComputerName.TabIndex = 1;
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
            this.panel2.Location = new System.Drawing.Point(13, 64);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(354, 150);
            this.panel2.TabIndex = 4;
            // 
            // txtCurrentPwdExpiration
            // 
            this.txtCurrentPwdExpiration.Location = new System.Drawing.Point(3, 70);
            this.txtCurrentPwdExpiration.Name = "txtCurrentPwdExpiration";
            this.txtCurrentPwdExpiration.ReadOnly = true;
            this.txtCurrentPwdExpiration.Size = new System.Drawing.Size(282, 20);
            this.txtCurrentPwdExpiration.TabIndex = 6;
            // 
            // lblCurrentPwdExpiration
            // 
            this.lblCurrentPwdExpiration.AutoSize = true;
            this.lblCurrentPwdExpiration.Location = new System.Drawing.Point(4, 53);
            this.lblCurrentPwdExpiration.Name = "lblCurrentPwdExpiration";
            this.lblCurrentPwdExpiration.Size = new System.Drawing.Size(89, 13);
            this.lblCurrentPwdExpiration.TabIndex = 5;
            this.lblCurrentPwdExpiration.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("PasswordExpires");
            // 
            // btnForceExpiration
            // 
            this.btnForceExpiration.Location = new System.Drawing.Point(291, 120);
            this.btnForceExpiration.Name = "btnForceExpiration";
            this.btnForceExpiration.Size = new System.Drawing.Size(63, 23);
            this.btnForceExpiration.TabIndex = 4;
            this.btnForceExpiration.Text = "Set";
            this.btnForceExpiration.UseVisualStyleBackColor = true;
            this.btnForceExpiration.Click += new System.EventHandler(this.btnForceExpiration_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 103);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(278, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("NewExpirationTime");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 4);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("Password");
            // 
            // txtPassword
            // 
            this.txtPassword.Location = new System.Drawing.Point(3, 26);
            this.txtPassword.Name = "txtPassword";
            this.txtPassword.ReadOnly = true;
            this.txtPassword.Size = new System.Drawing.Size(282, 20);
            this.txtPassword.TabIndex = 2;
            this.txtPassword.TabStop = false;
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(304, 219);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(63, 23);
            this.btnExit.TabIndex = 5;
            this.btnExit.Text = "Exit";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblStatus});
            this.statusStrip.Location = new System.Drawing.Point(0, 249);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(379, 22);
            this.statusStrip.TabIndex = 7;
            this.statusStrip.Text = "statusStrip1";
            // 
            // lblStatus
            // 
            this.lblStatus.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(0, 17);
            // 
            // Form1
            // 
            this.AcceptButton = this.btnSearch;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(379, 271);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = (new System.Resources.ResourceManager("Strings", typeof(Form1).Assembly)).GetString("AppTitle");
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

