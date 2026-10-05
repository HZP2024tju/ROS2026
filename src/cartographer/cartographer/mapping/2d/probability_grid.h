/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CARTOGRAPHER_MAPPING_2D_PROBABILITY_GRID_H_
#define CARTOGRAPHER_MAPPING_2D_PROBABILITY_GRID_H_

#include <vector>

#include "cartographer/common/port.h"
#include "cartographer/mapping/2d/grid_2d.h"
#include "cartographer/mapping/2d/map_limits.h"
#include "cartographer/mapping/2d/xy_index.h"

namespace cartographer {
namespace mapping {

// 表示一个 2D 概率栅格地图。
class ProbabilityGrid : public Grid2D {
  //Grind2D是一个提供通用接口的父类
 public:
  // 构造函数 1：从地图范围 + 值转换表创建
  explicit ProbabilityGrid(const MapLimits& limits,
                           ValueConversionTables* conversion_tables);
  // 构造函数 2：从 protobuf 反序列化创建
  explicit ProbabilityGrid(const proto::Grid2D& proto,
                           ValueConversionTables* conversion_tables);//explicit:禁止隐式转换 防止犯唐

  // 把 cell_index 处的格子的概率设为 probability。
  // 只允许在该格子之前是"未知"时调用。
  void SetProbability(const Eigen::Array2i& cell_index,
                      const float probability);

  // 把 ComputeLookupTableToApplyOdds() 生成的 odds 查找表
  // 应用到 cell_index 处的格子上（如果该格子还没被更新过）。
  // 同一个格子的多次更新会被忽略，直到调用 FinishUpdate() 为止。
  // 如果格子被更新了，返回 true。
  //
  // 如果这是对该格子的第一次 ApplyOdds() 调用，
  // 它的值会被设为 odds 对应的概率。
  bool ApplyLookupTable(const Eigen::Array2i& cell_index,
                        const std::vector<uint16>& table);

  // 返回该栅格的类型
  GridType GetGridType() const override;

  // 返回 cell_index 处格子的概率
  float GetProbability(const Eigen::Array2i& cell_index) const;
  // 序列化为 protobuf
  proto::Grid2D ToProto() const override;
  std::unique_ptr<Grid2D> ComputeCroppedGrid() const override;
  // 绘制到子图纹理（用于可视化/查询）
  bool DrawToSubmapTexture(
      proto::SubmapQuery::Response::SubmapTexture* const texture,
      transform::Rigid3d local_pose) const override;

 private:
  // 值转换表：概率 ↔ uint16 之间的映射
  ValueConversionTables* conversion_tables_;
};

}  // namespace mapping
}  // namespace cartographer

#endif  // CARTOGRAPHER_MAPPING_2D_PROBABILITY_GRID_H_
