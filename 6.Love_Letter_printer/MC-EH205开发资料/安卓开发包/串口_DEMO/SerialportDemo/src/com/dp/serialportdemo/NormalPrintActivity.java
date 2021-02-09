package com.dp.serialportdemo;

import java.io.IOException;
import java.io.InputStream;

import com.dp.serialportdemo.util.Toast_Util;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;

public class NormalPrintActivity extends Activity {


	private Button bt_text,bt_barcode,bt_qr,bt_image,bt_cuttall,bt_cutting,bt_queueprint,bt_printdemo1,bt_printdemo2,bt_textimage;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_normal);
		bt_text=(Button)findViewById(R.id.bt_text);
		bt_barcode=(Button)findViewById(R.id.bt_barcode);
		bt_qr=(Button)findViewById(R.id.bt_qr);
		bt_image=(Button)findViewById(R.id.bt_image);
		bt_cuttall=(Button)findViewById(R.id.bt_cuttall);
		bt_cutting=(Button)findViewById(R.id.bt_cutting);
		bt_queueprint=(Button)findViewById(R.id.bt_queueprint);
		bt_printdemo1=(Button)findViewById(R.id.bt_printdemo1);
		bt_printdemo2=(Button)findViewById(R.id.bt_printdemo2);
		bt_textimage=(Button)findViewById(R.id.bt_textimage);
		//打印文本
		bt_text.setOnClickListener(new OnClickListener() {			
			@SuppressWarnings("static-access")
			@Override
			public void onClick(View arg0) {
				//第一个参数   打印文本内容
				//第二个参数   字体对齐方式 默认1左对齐   2居中对齐  3靠右对齐
				//第三个参数   字体大小1默认正常字体   2字体放大一倍    3字体倍宽   4 字体倍高
				//第四个参数   行距 有效值未 10 20 30 40 50 60
				MainActivity.sf.PrintText("默认文字居左",1,1,20);
				MainActivity.sf.PrintText("文字居中",2,1,10);
				MainActivity.sf.PrintText("文字居右",3,1,20);
				MainActivity.sf.PrintText("正常字体大小",1,1,30);
				MainActivity.sf.PrintText("字体放大一倍",1,2,40);
				MainActivity.sf.PrintText("倍宽字体",1,3,30);
				MainActivity.sf.PrintText("倍高字体",1,4,30);
				MainActivity.sf.PrintText("_______________________",1,1,10);
				MainActivity.sf.PrintText("=======================",1,1,10);
				MainActivity.sf.PrintText("***********************",1,1,10);
				MainActivity.sf.PrintText("-----------------------",1,1,10);
				MainActivity.sf.PrintText(" ",1,1,10);
				MainActivity.sf.PrintText("",1,1,50);
				MainActivity.sf.PrintText("",1,1,10);
				MainActivity.sf.PaperCut();
			}
		});
//		打印条码
		bt_barcode.setOnClickListener(new OnClickListener() {			
			@SuppressWarnings("static-access")
			@Override
			public void onClick(View arg0) {
				//第一个参数   打印条码内容，条码内容不能包含有中文字符，只能是英文字符或数字
				//第二个参数   条形码宽度范围值1-6
				//第三个参数  形码高度范围值10-200
				//第四个参数   条形码类型 范围值 1-9
				//第五个参数  条码文本显示位置 不打印文本0、条码上方打印1、条码下方打印2、条码上下方打印3
				//第六个参数  对齐方式 默认1左对齐   2居中对齐  3靠右对齐
				MainActivity.sf.PrintBarcode("5A3456A5",3,100,5,2,1);
				MainActivity.sf.PrintBarcode("6A3456A6",5,200,6,2,2);
				MainActivity.sf.PrintBarcode("8A3456A8",3,80,8,2,3);
				MainActivity.sf.PrintBarcode("9A3456A9",3,80,9,2,2);
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PrintText("",1,2,50);
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PaperCut();
			}
		});
		//打印二维码
		bt_qr.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				//第一个参数   打印二维码内容
				//第二个参数   二维码大小范围值2-9
				//第三个参数  对齐方式 默认1左对齐   2居中对齐  3靠右对齐
				MainActivity.sf.PrintQR("爱我中华", 2,0);
				MainActivity.sf.PrintQR("爱我中华", 3,0);
				MainActivity.sf.PrintQR("爱我中华", 4,1);
				MainActivity.sf.PrintQR("爱我中华", 5,2);
				MainActivity.sf.PrintQR("爱我中华", 6,3);	
				MainActivity.sf.PrintQR("爱我中华", 7,0);	
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PrintText("",1,2,50);
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PaperCut();
				
			}
		});
//		打印图片
		bt_image.setOnClickListener(new OnClickListener() {			
			@SuppressWarnings("static-access")
			@Override
			public void onClick(View arg0) {
				Bitmap bm = getImageFromAssetsFile("1.jpg");
				Bitmap bm2 = getImageFromAssetsFile("2.jpg");
				if (null != bm) {
					//第一个参数   bitmap 图片
					//第二个参数   图片宽度 ，宽度范围不能超过纸张的宽度，最大不超过576
					//第三个参数  灰度值 范围 0-150
					//第四个参数   对齐方式 默认1左对齐   2居中对齐  3靠右对齐
					MainActivity.sf.PrintImage(bm,384,0,2);//打印黑白色图片 print black and white pictures
					MainActivity.sf.PrintImage(bm2,384,0,2);
					MainActivity.sf.PrintText("",1,2,50);
					MainActivity.sf.PrintPicture(bm2,384,2);//用于彩色转灰度图片打印 For color to grayscale image printing
					MainActivity.sf.PrintText("",1,2,50);
					MainActivity.sf.PrintText("",1,2,30);
					MainActivity.sf.PaperCut();
				}else {
					Toast_Util.ToastString(getApplicationContext(), R.string.no_printed_pictures);//没有打印图片
				}
			}
		});
		bt_textimage.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				MainActivity.sf.PrintTextImage("中国人民",184,20,1);
				MainActivity.sf.PrintTextImage("中国人民",184,30,1);
				MainActivity.sf.PrintTextImage("中国人民",184,40,1);
				MainActivity.sf.PrintTextImage("中国人民",184,50,1);
			}
		});
		bt_queueprint.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				
				int interval1=0;//第一列距离
				int interval2=180;//第二列距离
				int interval3=250;//第三列距离
				int interval4=320;//第四列距离
				int right=70;//第四靠右对齐距离
				MainActivity.sf.PrintText("第四列不靠右对齐",1,1,10);
				MainActivity.sf.Printcolumncontent("商品名称",interval1,"单价",interval2,"数量",interval3," 金额",interval4);
				MainActivity.sf.Printcolumncontent("统一冰红茶",interval1,"1.5",interval2,"1",interval3,"1.5",interval4);
				MainActivity.sf.Printcolumncontent("娃哈哈牛奶",interval1,"25",interval2,"1",interval3,"25.0",interval4);
				MainActivity.sf.Printcolumncontent("篮球",interval1,"108",interval2,"1",interval3,"108.0",interval4);
				
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PrintText("第四列靠右对齐",1,1,10);
				MainActivity.sf.Printcolumncontent("商品名称",interval1,"单价",interval2,"数量",interval3," 金额",interval4);
				MainActivity.sf.Printcolumncontent("统一冰红茶",interval1,"1.5",interval2,"1",interval3,"1.5",interval4,right);
				MainActivity.sf.Printcolumncontent("娃哈哈牛奶",interval1,"25",interval2,"1",interval3,"25.0",interval4,right);
				MainActivity.sf.Printcolumncontent("篮球",interval1,"108",interval2,"1",interval3,"108.0",interval4,right);
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PrintText("",1,2,30);
				MainActivity.sf.PaperCut();
			}
		});
		
		bt_cutting.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					MainActivity.sf.PaperCut();//半切
				}
			});
		bt_cuttall.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				MainActivity.sf.PaperAllCut();//全切
			}
		});
		
		
		bt_printdemo1.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
			
				demo();
			}
		});
		bt_printdemo2.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
				demo3();
			}
		});
	}

	/**
	 * 打印测试页面
	 * @param v
	 */
	public void printtestpage(View v)
	{		
		MainActivity.sf.PrintTestPage();
	}
	
	/**
	 * 检查缺纸
	 * 
	 */
	@SuppressWarnings("static-access")
	public void checkpage(View v)
	{
		MainActivity.sf.Check_Paper();
		
	}
	
	/**
	 * 从Assets中读取图片
	 */
	private Bitmap getImageFromAssetsFile(String fileName) {
		Bitmap image = null;
		AssetManager am = getResources().getAssets();
		try {
			InputStream is = am.open(fileName);
			image = BitmapFactory.decodeStream(is);
			is.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return image;

	}
	
	public void back(View v)
	{
		finish();
	}
	
	String line2="================================";
	String line="================================================";
	private  void demo()
	{
		int interval1=0;//第一列距离
		int interval2=180;//第二列距离
		int interval3=250;//第三列距离
		int interval4=300;//第四列距离
		int right=100;//第四靠右对齐距离
		MainActivity.sf.PrintText("\r\n欢迎光临永辉超市-碧江广场店",2,1,30);
		MainActivity.sf.PrintText("机  号：ZZ37      收银员：6001",1,1,20);
		MainActivity.sf.PrintText("小票号：517266",1,1,20);
		MainActivity.sf.PrintText("流水号:201809072006200zz7063",1,1,20);
		MainActivity.sf.PrintText("日  期：2018-09-07  20:03:06",1,1,20);
		MainActivity.sf.PrintText(line2,1,1,20);
		
		MainActivity.sf.Printcolumncontent("商品名称",interval1,"单价",interval2,"数量",interval3,"  金额",interval4);
		MainActivity.sf.Printcolumncontent("土鸡蛋(折价)\r\n2306068004096",interval1,"7.0",interval2,"1份",interval3,"7.00",interval4,right);
		MainActivity.sf.Printcolumncontent("毛豆(精品)\r\n2301391003006",interval1,"4.0",interval2,"1份",interval3,"4.00",interval4,right);
		MainActivity.sf.Printcolumncontent("香蕉\r\n2304947002904",interval1,"3.0",interval2,"5",interval3,"15.00",interval4,right);
		MainActivity.sf.Printcolumncontent("松茂苏式椒盐月饼358g\r\n692272419484",interval1,"17.5",interval2,"2.5",interval3,"43.75",interval4,right);
		MainActivity.sf.Printcolumncontent("普通促销折扣",interval1,"",interval2,"",interval3,"6.00",interval4,right);
		MainActivity.sf.PrintText(line2,1,1,20);
		MainActivity.sf.Printcolumncontent("应付款：63.75",interval1,"",interval2,"件数：4",interval3,"",interval4);
		MainActivity.sf.Printcolumncontent("实付款：63.75",interval1,"",interval2,"找回：0.00",interval3,"",interval4,right);
		MainActivity.sf.PrintText("\r\n其中  支付宝  20.79",1,1,20);
		MainActivity.sf.PrintText("支付宝优惠金额5.00",1,1,20);
		MainActivity.sf.PrintText("支付宝实际扣款金额：15.79",1,1,20);
		MainActivity.sf.PrintText("支付宝订单号:20180907220014527288811109",1,1,20);
		MainActivity.sf.PrintText("会员卡号：1-01430770746",1,1,20);
		MainActivity.sf.PrintText(line2,1,1,20);
		MainActivity.sf.PrintText("客服电话：021-64350618",1,1,20);
		MainActivity.sf.PrintText("请当面清点所购商品和找零，并保管好收银小票以做开发票，退换货凭证，谢谢惠顾。",1,1,20);
		MainActivity.sf.PrintText("",1,1,30);
		MainActivity.sf.PrintText("开票号：20180907220014527288811109",1,1,20);
		MainActivity.sf.PrintText("友情提示：开票日期以开具当天为准，建议您消费48小时内进行开票据申请并及时下载电子发票电子发票下载时间自开发票之日起60日内有效",1,1,20);
		MainActivity.sf.PrintQR("永辉超市", 6,2);
		MainActivity.sf.PrintText("",1,1,10);
		MainActivity.sf.PrintText("",1,1,50);
		MainActivity.sf.PrintText("",1,1,30);
		MainActivity.sf.PaperCut();
	}
	private  void demo3()
	{
		int interval1=0;//第一列距离
		int interval2=220;//第二列距离
		int interval3=330;//第三列距离
		int interval4=400;//第四列距离
		int right=120;//第四靠右对齐距离
		MainActivity.sf.PrintText("\r\n欢迎光临永辉超市-碧江广场店",2,1,30);
		MainActivity.sf.PrintText("机  号：ZZ37         收银员：6001",1,1,20);
		MainActivity.sf.PrintText("小票号：517266",1,1,20);
		MainActivity.sf.PrintText("流水号:201809072006200zz7063",1,1,20);
		MainActivity.sf.PrintText("日  期：2018-09-07  20:03:06",1,1,20);
		MainActivity.sf.PrintText(line,1,1,20);
		

		MainActivity.sf.Printcolumncontent("商品名称",interval1,"单价",interval2,"数量",interval3,"   金额",interval4);
		MainActivity.sf.Printcolumncontent("土鸡蛋(折价)\r\n2306068004096",interval1,"7.00",interval2,"1份",interval3,"7.00",interval4,right);
		MainActivity.sf.Printcolumncontent("毛豆(精品)\r\n2301391003006",interval1,"4.00",interval2,"1份",interval3,"4.00",interval4,right);
		MainActivity.sf.Printcolumncontent("香蕉\r\n2304947002904",interval1,"3.00",interval2,"5",interval3,"15.00",interval4,right);
		MainActivity.sf.Printcolumncontent("松茂苏式椒盐月饼358g\r\n692272419484",interval1,"17.50",interval2,"2.5",interval3,"43.75",interval4,right);
		MainActivity.sf.Printcolumncontent("普通促销折扣",interval1,"",interval2,"",interval3,"6.00",interval4,right);
		MainActivity.sf.PrintText(line,1,1,20);
		MainActivity.sf.Printcolumncontent("应付款：63.75",interval1,"",interval2,"件数：4",interval3,"",interval4);
		MainActivity.sf.Printcolumncontent("实付款：63.75",interval1,"",interval2,"找回：0.00",interval3,"",interval4,right);
		
		MainActivity.sf.PrintText("\r\n其中  支付宝  20.79",1,1,20);
		MainActivity.sf.PrintText("支付宝优惠金额5.00",1,1,20);
		MainActivity.sf.PrintText("支付宝实际扣款金额：15.79",1,1,20);
		MainActivity.sf.PrintText("支付宝订单号:20180907220014527288811109",1,1,20);
		MainActivity.sf.PrintText("会员卡号：1-01430770746",1,1,20);
		MainActivity.sf.PrintText(line,1,1,20);
		MainActivity.sf.PrintText("客服电话：021-64350618",1,1,20);
		MainActivity.sf.PrintText("请当面清点所购商品和找零，并保管好收银小票以做开发票，退换货凭证，谢谢惠顾。",1,1,20);
		MainActivity.sf.PrintText("",1,1,30);
		MainActivity.sf.PrintText("开票号：20180907220014527288811109",1,1,20);
		MainActivity.sf.PrintText("友情提示：开票日期以开具当天为准，建议您消费48小时内进行开票据申请并及时下载电子发票电子发票下载时间自开发票之日起60日内有效",1,1,20);
		MainActivity.sf.PrintQR("永辉超市", 8,2);
		MainActivity.sf.PrintText("",1,1,10);
		MainActivity.sf.PrintText("",1,1,50);
		MainActivity.sf.PrintText("",1,1,30);
		MainActivity.sf.PaperCut();
	}
	
}
