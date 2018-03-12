namespace Indoor_Positioning_GUI
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
            this.latlong_checkBox = new System.Windows.Forms.CheckBox();
            this.errorelipses_checkBox = new System.Windows.Forms.CheckBox();
            this.network_checkBox = new System.Windows.Forms.CheckBox();
            this.latitude_textBox = new System.Windows.Forms.TextBox();
            this.longitude_textBox = new System.Windows.Forms.TextBox();
            this.error_textBox = new System.Windows.Forms.TextBox();
            this.latitude_label = new System.Windows.Forms.Label();
            this.longitude_label = new System.Windows.Forms.Label();
            this.error_label = new System.Windows.Forms.Label();
            this.generate_button = new System.Windows.Forms.Button();
            this.gMap = new GMap.NET.WindowsForms.GMapControl();
            this.error2_textBox = new System.Windows.Forms.TextBox();
            this.longerror_label = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.marker_Label = new System.Windows.Forms.Label();
            this.tableGridView = new System.Windows.Forms.DataGridView();
            ((System.ComponentModel.ISupportInitialize)(this.tableGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // latlong_checkBox
            // 
            this.latlong_checkBox.AutoSize = true;
            this.latlong_checkBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.latlong_checkBox.Location = new System.Drawing.Point(28, 590);
            this.latlong_checkBox.Name = "latlong_checkBox";
            this.latlong_checkBox.Size = new System.Drawing.Size(127, 22);
            this.latlong_checkBox.TabIndex = 1;
            this.latlong_checkBox.Text = "Show ToolTips";
            this.latlong_checkBox.UseVisualStyleBackColor = true;
            this.latlong_checkBox.CheckedChanged += new System.EventHandler(this.latlong_checkBox_CheckedChanged);
            // 
            // errorelipses_checkBox
            // 
            this.errorelipses_checkBox.AutoSize = true;
            this.errorelipses_checkBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.errorelipses_checkBox.Location = new System.Drawing.Point(310, 590);
            this.errorelipses_checkBox.Name = "errorelipses_checkBox";
            this.errorelipses_checkBox.Size = new System.Drawing.Size(155, 22);
            this.errorelipses_checkBox.TabIndex = 2;
            this.errorelipses_checkBox.Text = "Show Error Elipses";
            this.errorelipses_checkBox.UseVisualStyleBackColor = true;
            this.errorelipses_checkBox.CheckedChanged += new System.EventHandler(this.errorelipses_checkBox_CheckedChanged);
            // 
            // network_checkBox
            // 
            this.network_checkBox.AutoSize = true;
            this.network_checkBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.network_checkBox.Location = new System.Drawing.Point(612, 590);
            this.network_checkBox.Name = "network_checkBox";
            this.network_checkBox.Size = new System.Drawing.Size(118, 22);
            this.network_checkBox.TabIndex = 3;
            this.network_checkBox.Text = "Network View";
            this.network_checkBox.UseVisualStyleBackColor = true;
            this.network_checkBox.CheckedChanged += new System.EventHandler(this.network_checkBox_CheckedChanged);
            // 
            // latitude_textBox
            // 
            this.latitude_textBox.Location = new System.Drawing.Point(1452, 32);
            this.latitude_textBox.Name = "latitude_textBox";
            this.latitude_textBox.Size = new System.Drawing.Size(100, 20);
            this.latitude_textBox.TabIndex = 4;
            this.latitude_textBox.TextChanged += new System.EventHandler(this.latitude_textBox_TextChanged);
            // 
            // longitude_textBox
            // 
            this.longitude_textBox.Location = new System.Drawing.Point(1452, 58);
            this.longitude_textBox.Name = "longitude_textBox";
            this.longitude_textBox.Size = new System.Drawing.Size(100, 20);
            this.longitude_textBox.TabIndex = 5;
            this.longitude_textBox.TextChanged += new System.EventHandler(this.longitude_textBox_TextChanged);
            // 
            // error_textBox
            // 
            this.error_textBox.Location = new System.Drawing.Point(1452, 85);
            this.error_textBox.Name = "error_textBox";
            this.error_textBox.Size = new System.Drawing.Size(100, 20);
            this.error_textBox.TabIndex = 6;
            this.error_textBox.TextChanged += new System.EventHandler(this.error_textBox_TextChanged);
            // 
            // latitude_label
            // 
            this.latitude_label.AutoSize = true;
            this.latitude_label.Location = new System.Drawing.Point(1394, 32);
            this.latitude_label.Name = "latitude_label";
            this.latitude_label.Size = new System.Drawing.Size(45, 13);
            this.latitude_label.TabIndex = 7;
            this.latitude_label.Text = "Latitude";
            this.latitude_label.Click += new System.EventHandler(this.latitude_label_Click);
            // 
            // longitude_label
            // 
            this.longitude_label.AutoSize = true;
            this.longitude_label.Location = new System.Drawing.Point(1394, 61);
            this.longitude_label.Name = "longitude_label";
            this.longitude_label.Size = new System.Drawing.Size(54, 13);
            this.longitude_label.TabIndex = 8;
            this.longitude_label.Text = "Longitude";
            this.longitude_label.Click += new System.EventHandler(this.longitude_label_Click);
            // 
            // error_label
            // 
            this.error_label.AutoSize = true;
            this.error_label.Location = new System.Drawing.Point(1394, 88);
            this.error_label.Name = "error_label";
            this.error_label.Size = new System.Drawing.Size(47, 13);
            this.error_label.TabIndex = 9;
            this.error_label.Text = "Lat Error";
            this.error_label.Click += new System.EventHandler(this.error_label_Click);
            // 
            // generate_button
            // 
            this.generate_button.Location = new System.Drawing.Point(1466, 137);
            this.generate_button.Name = "generate_button";
            this.generate_button.Size = new System.Drawing.Size(75, 23);
            this.generate_button.TabIndex = 10;
            this.generate_button.Text = "Generate";
            this.generate_button.UseVisualStyleBackColor = true;
            this.generate_button.Click += new System.EventHandler(this.generate_button_Click);
            // 
            // gMap
            // 
            this.gMap.Bearing = 0F;
            this.gMap.CanDragMap = true;
            this.gMap.EmptyTileColor = System.Drawing.Color.Navy;
            this.gMap.GrayScaleMode = false;
            this.gMap.HelperLineOption = GMap.NET.WindowsForms.HelperLineOptions.DontShow;
            this.gMap.LevelsKeepInMemmory = 5;
            this.gMap.Location = new System.Drawing.Point(-1, 0);
            this.gMap.MarkersEnabled = true;
            this.gMap.MaxZoom = 22;
            this.gMap.MinZoom = 2;
            this.gMap.MouseWheelZoomType = GMap.NET.MouseWheelZoomType.MousePositionAndCenter;
            this.gMap.Name = "gMap";
            this.gMap.NegativeMode = false;
            this.gMap.PolygonsEnabled = true;
            this.gMap.RetryLoadTile = 0;
            this.gMap.RoutesEnabled = true;
            this.gMap.ScaleMode = GMap.NET.WindowsForms.ScaleModes.Integer;
            this.gMap.SelectedAreaFillColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(65)))), ((int)(((byte)(105)))), ((int)(((byte)(225)))));
            this.gMap.ShowTileGridLines = false;
            this.gMap.Size = new System.Drawing.Size(755, 584);
            this.gMap.TabIndex = 11;
            this.gMap.Zoom = 17D;
            this.gMap.Load += new System.EventHandler(this.gMap_Load);
            // 
            // error2_textBox
            // 
            this.error2_textBox.Location = new System.Drawing.Point(1452, 111);
            this.error2_textBox.Name = "error2_textBox";
            this.error2_textBox.Size = new System.Drawing.Size(100, 20);
            this.error2_textBox.TabIndex = 12;
            this.error2_textBox.TextChanged += new System.EventHandler(this.error2_textBox_TextChanged);
            // 
            // longerror_label
            // 
            this.longerror_label.AutoSize = true;
            this.longerror_label.Location = new System.Drawing.Point(1392, 114);
            this.longerror_label.Name = "longerror_label";
            this.longerror_label.Size = new System.Drawing.Size(56, 13);
            this.longerror_label.TabIndex = 13;
            this.longerror_label.Text = "Long Error";
            this.longerror_label.Click += new System.EventHandler(this.longerror_label_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(1452, 6);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 14;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // marker_Label
            // 
            this.marker_Label.AutoSize = true;
            this.marker_Label.Location = new System.Drawing.Point(1394, 9);
            this.marker_Label.Name = "marker_Label";
            this.marker_Label.Size = new System.Drawing.Size(54, 13);
            this.marker_Label.TabIndex = 15;
            this.marker_Label.Text = "Marker ID";
            // 
            // tableGridView
            // 
            this.tableGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.tableGridView.Location = new System.Drawing.Point(760, 0);
            this.tableGridView.Name = "tableGridView";
            this.tableGridView.Size = new System.Drawing.Size(626, 584);
            this.tableGridView.TabIndex = 16;
            this.tableGridView.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.tableGridView_CellContentClick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(1364, 625);
            this.Controls.Add(this.tableGridView);
            this.Controls.Add(this.marker_Label);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.longerror_label);
            this.Controls.Add(this.error2_textBox);
            this.Controls.Add(this.gMap);
            this.Controls.Add(this.generate_button);
            this.Controls.Add(this.error_label);
            this.Controls.Add(this.longitude_label);
            this.Controls.Add(this.latitude_label);
            this.Controls.Add(this.error_textBox);
            this.Controls.Add(this.longitude_textBox);
            this.Controls.Add(this.latitude_textBox);
            this.Controls.Add(this.network_checkBox);
            this.Controls.Add(this.errorelipses_checkBox);
            this.Controls.Add(this.latlong_checkBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "Form1";
            this.Text = "Indoor Positioning GUI";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.tableGridView)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.CheckBox latlong_checkBox;
        private System.Windows.Forms.CheckBox errorelipses_checkBox;
        private System.Windows.Forms.CheckBox network_checkBox;
        private System.Windows.Forms.TextBox latitude_textBox;
        private System.Windows.Forms.TextBox longitude_textBox;
        private System.Windows.Forms.TextBox error_textBox;
        private System.Windows.Forms.Label latitude_label;
        private System.Windows.Forms.Label longitude_label;
        private System.Windows.Forms.Button generate_button;
        private GMap.NET.WindowsForms.GMapControl gMap;
        private System.Windows.Forms.Label error_label;
        private System.Windows.Forms.TextBox error2_textBox;
        private System.Windows.Forms.Label longerror_label;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label marker_Label;
        private System.Windows.Forms.DataGridView tableGridView;
    }
}

