// Copyright (C) 2010 Tim Moore (timoore33@gmail.com)
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "ConditionNode.hxx"

#include <simgear/math/SGMath.hxx>

namespace simgear
{
using namespace osg;

ConditionNode::ConditionNode()
{
}

ConditionNode::ConditionNode(const ConditionNode& rhs, const CopyOp& op)
    : Group(rhs, op), _condition(rhs._condition)
{
}

ConditionNode::~ConditionNode()
{
}

void ConditionNode::traverse(NodeVisitor& nv)
{
    if (nv.getTraversalMode() == NodeVisitor::TRAVERSE_ACTIVE_CHILDREN) {
        unsigned numChildren = getNumChildren();
        if (numChildren == 0)
            return;
        if (!_condition || _condition->test())
            getChild(0)->accept(nv);
        else if (numChildren > 1)
            getChild(1)->accept(nv);
        else
            return;
    } else {
        Group::traverse(nv);
    }
}

}