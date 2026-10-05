#ifndef SCANMATCHER_H
#define SCANMATCHER_H

#include "gmapping/scanmatcher/icp.h"
#include "gmapping/scanmatcher/smmap.h"
#include <gmapping/utils/macro_params.h>
#include <gmapping/utils/stat.h>
#include <iostream>
#include <gmapping/utils/gvalues.h>
#include <gmapping/scanmatcher/scanmatcher_export.h>
#define LASER_MAXBEAMS 2048

namespace GMapping {

class SCANMATCHER_EXPORT ScanMatcher{
	public:
		typedef Covariance3 CovarianceMatrix;
		
		ScanMatcher();
		~ScanMatcher();
		double icpOptimize(OrientedPoint& pnew, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		double optimize(OrientedPoint& pnew, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		double optimize(OrientedPoint& mean, CovarianceMatrix& cov, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		
		double   registerScan(ScanMatcherMap& map, const OrientedPoint& p, const double* readings);
		void setLaserParameters
			(unsigned int beams, double* angles, const OrientedPoint& lpose);
		void setMatchingParameters
			(double urange, double range, double sigma, int kernsize, double lopt, double aopt, int iterations, double likelihoodSigma=1, unsigned int likelihoodSkip=0 );
		void invalidateActiveArea();
		void computeActiveArea(ScanMatcherMap& map, const OrientedPoint& p, const double* readings);

		inline double icpStep(OrientedPoint & pret, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		inline double score(const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		inline unsigned int likelihoodAndScore(double& s, double& l, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		double likelihood(double& lmax, OrientedPoint& mean, CovarianceMatrix& cov, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings);
		double likelihood(double& _lmax, OrientedPoint& _mean, CovarianceMatrix& _cov, const ScanMatcherMap& map, const OrientedPoint& p, Gaussian3& odometry, const double* readings, double gain=180.);
		inline const double* laserAngles() const { return m_laserAngles; }
		inline unsigned int laserBeams() const { return m_laserBeams; }
		
		static const double nullLikelihood;
	protected:
		//state of the matcher
		bool m_activeAreaComputed;
		
		/**laser parameters*/
		unsigned int m_laserBeams;
		double       m_laserAngles[LASER_MAXBEAMS];
		//OrientedPoint m_laserPose;
		PARAM_SET_GET(OrientedPoint, laserPose, protected, public, public)
		PARAM_SET_GET(double, laserMaxRange, protected, public, public)
		/**scan_matcher parameters*/
		PARAM_SET_GET(double, usableRange, protected, public, public)
		PARAM_SET_GET(double, gaussianSigma, protected, public, public)
		PARAM_SET_GET(double, likelihoodSigma, protected, public, public)
		PARAM_SET_GET(int,    kernelSize, protected, public, public)
		PARAM_SET_GET(double, optAngularDelta, protected, public, public)
		PARAM_SET_GET(double, optLinearDelta, protected, public, public)
		PARAM_SET_GET(unsigned int, optRecursiveIterations, protected, public, public)
		PARAM_SET_GET(unsigned int, likelihoodSkip, protected, public, public)
		PARAM_SET_GET(double, llsamplerange, protected, public, public)
		PARAM_SET_GET(double, llsamplestep, protected, public, public)
		PARAM_SET_GET(double, lasamplerange, protected, public, public)
		PARAM_SET_GET(double, lasamplestep, protected, public, public)
		PARAM_SET_GET(bool, generateMap, protected, public, public)
		PARAM_SET_GET(double, enlargeStep, protected, public, public)
		PARAM_SET_GET(double, fullnessThreshold, protected, public, public)
		PARAM_SET_GET(double, angularOdometryReliability, protected, public, public)
		PARAM_SET_GET(double, linearOdometryReliability, protected, public, public)
		PARAM_SET_GET(double, freeCellRatio, protected, public, public)
		PARAM_SET_GET(unsigned int, initialBeamsSkip, protected, public, public)

		// allocate this large array only once
		IntPoint* m_linePoints;
};

inline double ScanMatcher::icpStep(OrientedPoint & pret, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const{
	const double * angle=m_laserAngles+m_initialBeamsSkip;
	OrientedPoint lp=p;
	lp.x+=cos(p.theta)*m_laserPose.x-sin(p.theta)*m_laserPose.y;
	lp.y+=sin(p.theta)*m_laserPose.x+cos(p.theta)*m_laserPose.y;
	lp.theta+=m_laserPose.theta;
	unsigned int skip=0;
	double freeDelta=map.getDelta()*m_freeCellRatio;
	std::list<PointPair> pairs;
	
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++){
		skip++;
		skip=skip>m_likelihoodSkip?0:skip;
		if (*r>m_usableRange||*r==0.0) continue;
		if (skip) continue;
		Point phit=lp;
		phit.x+=*r*cos(lp.theta+*angle);
		phit.y+=*r*sin(lp.theta+*angle);
		IntPoint iphit=map.world2map(phit);
		Point pfree=lp;
		pfree.x+=(*r-map.getDelta()*freeDelta)*cos(lp.theta+*angle);
		pfree.y+=(*r-map.getDelta()*freeDelta)*sin(lp.theta+*angle);
 		pfree=pfree-phit;
		IntPoint ipfree=map.world2map(pfree);
		bool found=false;
		Point bestMu(0.,0.);
		Point bestCell(0.,0.);
		for (int xx=-m_kernelSize; xx<=m_kernelSize; xx++)
		for (int yy=-m_kernelSize; yy<=m_kernelSize; yy++){
			IntPoint pr=iphit+IntPoint(xx,yy);
			IntPoint pf=pr+ipfree;
			//AccessibilityState s=map.storage().cellState(pr);
			//if (s&Inside && s&Allocated){
				const PointAccumulator& cell=map.cell(pr);
				const PointAccumulator& fcell=map.cell(pf);
				if (((double)cell )> m_fullnessThreshold && ((double)fcell )<m_fullnessThreshold){
					Point mu=phit-cell.mean();
					if (!found){
						bestMu=mu;
						bestCell=cell.mean();
						found=true;
					}else
						if((mu*mu)<(bestMu*bestMu)){
							bestMu=mu;
							bestCell=cell.mean();
						} 
						
				}
			//}
		}
		if (found){
			pairs.push_back(std::make_pair(phit, bestCell));
			//std::cerr << "(" << phit.x-bestCell.x << "," << phit.y-bestCell.y << ") ";
		}
		//std::cerr << std::endl;
	}
	
	OrientedPoint result(0,0,0);
	//double icpError=icpNonlinearStep(result,pairs);
	std::cerr << "result(" << pairs.size() << ")=" << result.x << " " << result.y << " " << result.theta << std::endl;
	pret.x=p.x+result.x;
	pret.y=p.y+result.y;
	pret.theta=p.theta+result.theta;
	pret.theta=atan2(sin(pret.theta), cos(pret.theta));
	return score(map, p, readings);
}

inline double ScanMatcher::score(const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const{
	double s=0;					//此处的map是粒子的地图而不是全局地图
	const double * angle=m_laserAngles+m_initialBeamsSkip; //angle是实际测量起始角度
	OrientedPoint lp=p;
	lp.x+=cos(p.theta)*m_laserPose.x-sin(p.theta)*m_laserPose.y;
	lp.y+=sin(p.theta)*m_laserPose.x+cos(p.theta)*m_laserPose.y;//此处m_laserpose是雷达相对机器人中心的位置
	lp.theta+=m_laserPose.theta;//得到雷达的绝对位姿
	unsigned int skip=0;
	double freeDelta=map.getDelta()*m_freeCellRatio; 
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++){
		//哦真的np还有这种写法    环绕一圈
		skip++;
		skip=skip>m_likelihoodSkip?0:skip; //过滤器 是否每个光束都计算
		if (skip||*r>m_usableRange||*r==0.0) continue;  //r是当前激光测距长
		Point phit=lp;
		phit.x+=*r*cos(lp.theta+*angle);//计算命中点的位置
		phit.y+=*r*sin(lp.theta+*angle);
		IntPoint iphit=map.world2map(phit);//利用tf变换 在地图上取点
		Point pfree=lp;
		pfree.x+=(*r-map.getDelta()*freeDelta)*cos(lp.theta+*angle); //这里有难绷的bug
		pfree.y+=(*r-map.getDelta()*freeDelta)*sin(lp.theta+*angle); //这里有更好的方法 比如查整条激光 防止穿墙
 		pfree=pfree-phit;  //得到一个雷霆自由点
		IntPoint ipfree=map.world2map(pfree);//利用tf变换 在地图上取点  整形点
		bool found=false;
		Point bestMu(0.,0.);
		for (int xx=-m_kernelSize; xx<=m_kernelSize; xx++)
		for (int yy=-m_kernelSize; yy<=m_kernelSize; yy++){
			IntPoint pr=iphit+IntPoint(xx,yy); //进行偏移 在设定窗口内查找障碍物  似然域模型
			IntPoint pf=pr+ipfree;
			//AccessibilityState s=map.storage().cellState(pr);
			//if (s&Inside && s&Allocated){
				const PointAccumulator& cell=map.cell(pr); //cell :栅格单元累加器 返回结构体
				const PointAccumulator& fcell=map.cell(pf);			//m_fullnessThreshold 这得多少才算多(占据)啊
				if (((double)cell )> m_fullnessThreshold && ((double)fcell )<m_fullnessThreshold)
				{ //哦真的牛批还有这种转换方式的 这里计算了可信度		如果栅格占据且来路为空
					Point mu=phit-cell.mean();
					if (!found){
						bestMu=mu; //设置最小偏差
						found=true;
					}else
						bestMu=(mu*mu)<(bestMu*bestMu)?mu:bestMu;
				}
			//}
		}
		if (found)
			s+=exp(-1./m_gaussianSigma*bestMu*bestMu);
			//累加计算高斯核来评分 (为什么要这样？)
	}
	return s;
}

inline unsigned int ScanMatcher::likelihoodAndScore(double& s, double& l, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const{
	using namespace std;
	l=0;
	s=0;
	const double * angle=m_laserAngles+m_initialBeamsSkip;
	OrientedPoint lp=p;
	lp.x+=cos(p.theta)*m_laserPose.x-sin(p.theta)*m_laserPose.y;
	lp.y+=sin(p.theta)*m_laserPose.x+cos(p.theta)*m_laserPose.y;
	lp.theta+=m_laserPose.theta;
	double noHit=nullLikelihood/(m_likelihoodSigma);//激光未命中时的惩罚值
	unsigned int skip=0;
	unsigned int c=0;
	double freeDelta=map.getDelta()*m_freeCellRatio;
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++){
		skip++;
		skip=skip>m_likelihoodSkip?0:skip;
		if (*r>m_usableRange) continue;//检查是否超出可用测距区域
		if (skip) continue;
		Point phit=lp;
		phit.x+=*r*cos(lp.theta+*angle);
		phit.y+=*r*sin(lp.theta+*angle);
		IntPoint iphit=map.world2map(phit);
		Point pfree=lp;
		pfree.x+=(*r-freeDelta)*cos(lp.theta+*angle);
		pfree.y+=(*r-freeDelta)*sin(lp.theta+*angle);
		pfree=pfree-phit;
		IntPoint ipfree=map.world2map(pfree);
		bool found=false;
		Point bestMu(0.,0.);
		for (int xx=-m_kernelSize; xx<=m_kernelSize; xx++)
		for (int yy=-m_kernelSize; yy<=m_kernelSize; yy++){
			IntPoint pr=iphit+IntPoint(xx,yy);
			IntPoint pf=pr+ipfree;
			//AccessibilityState s=map.storage().cellState(pr);
			//if (s&Inside && s&Allocated){
				const PointAccumulator& cell=map.cell(pr);
				const PointAccumulator& fcell=map.cell(pf);
				if (((double)cell )>m_fullnessThreshold && ((double)fcell )<m_fullnessThreshold){
					Point mu=phit-cell.mean();
					if (!found){
						bestMu=mu;
						found=true;
					}else
						bestMu=(mu*mu)<(bestMu*bestMu)?mu:bestMu;
				}
			//}	
		}
		if (found){
			s+=exp(-1./m_gaussianSigma*bestMu*bestMu);//计算匹配得分 梯度平缓
			c++;
		}
		if (!skip){
			double f=(-1./m_likelihoodSigma)*(bestMu*bestMu);//计算权重 似然方法 和得分差不多但是梯度尖锐适于区分
			l+=(found)?f:noHit;
		}
	}
	return c;
}

};

#endif
