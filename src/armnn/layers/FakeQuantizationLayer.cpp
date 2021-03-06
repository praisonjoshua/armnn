//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#include "FakeQuantizationLayer.hpp"

#include "LayerCloneBase.hpp"

#include <armnn/TypesUtils.hpp>
#include <backends/WorkloadData.hpp>
#include <backends/WorkloadFactory.hpp>

namespace armnn
{

FakeQuantizationLayer::FakeQuantizationLayer(const FakeQuantizationDescriptor& param, const char* name)
: LayerWithParameters(1, 1, LayerType::FakeQuantization, param, name)
{
}

std::unique_ptr<IWorkload> FakeQuantizationLayer::CreateWorkload(const Graph& graph,
                                                                const IWorkloadFactory& factory) const
{
    FakeQuantizationQueueDescriptor descriptor;
    return factory.CreateFakeQuantization(descriptor, PrepInfoAndDesc(descriptor, graph) );
}

FakeQuantizationLayer* FakeQuantizationLayer::Clone(Graph& graph) const
{
    return CloneBase<FakeQuantizationLayer>(graph, m_Param, GetName());
}

void FakeQuantizationLayer::ValidateTensorShapesFromInputs()
{
    VerifyLayerConnections(1, CHECK_LOCATION());

    auto inferredShapes = InferOutputShapes({ GetInputSlot(0).GetConnection()->GetTensorInfo().GetShape() });

    BOOST_ASSERT(inferredShapes.size() == 1);

    ConditionalThrowIfNotEqual<LayerValidationException>(
        "FakeQuantizationLayer: TensorShape set on OutputSlot[0] does not match the inferred shape.",
        GetOutputSlot(0).GetTensorInfo().GetShape(),
        inferredShapes[0]);
}

} // namespace armnn
