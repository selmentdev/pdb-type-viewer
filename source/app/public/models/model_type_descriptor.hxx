#pragma once
#include <QAbstractItemModel>
#include <ptv/pdb_abstract_type_member.hxx>
#include <ptv/pdb_type_descriptor.hxx>

namespace ptvapp::models
{
    enum class TypeDescriptorElementRole
    {
        Type,
        Name,
        Size,
        Offset,
        Kind,
        Bits,
        Padding,
        IsPadding,
        IsSpurious,
    };

    class TypeDescriptorElement
    {
    private:
        QList<TypeDescriptorElement*> m_Children;
        TypeDescriptorElement* m_Parent;
        ptv::pdb_abstract_type_member* m_Member;

    public:
        explicit TypeDescriptorElement(
            ptv::pdb_abstract_type_member* member,
            TypeDescriptorElement* parent = nullptr
        ) noexcept;

        ~TypeDescriptorElement() noexcept;

    public:
        void AppendChild(
            TypeDescriptorElement* child
        ) noexcept;

        TypeDescriptorElement* GetChild(
            int index
        ) const noexcept;

        int GetChildrenCount() const noexcept;

        QVariant data(
            TypeDescriptorElementRole role
        ) const noexcept;

        int GetRowIndex() const noexcept;

        TypeDescriptorElement* GetParent() const noexcept;

        void Clear() noexcept;

    public:
        ptv::pdb_abstract_type_member* GetMember() const noexcept
        {
            return m_Member;
        }
    };
}

namespace ptvapp::models
{
    class TypeDescriptorModel : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        TypeDescriptorElement* m_Root;
        std::unique_ptr<ptv::pdb_type_descriptor> m_CurrentDescriptor;

    public:
        explicit TypeDescriptorModel(
            QObject* parent = nullptr
        ) noexcept;

        ~TypeDescriptorModel() noexcept;

    public:
        void FromDescriptor(
            std::unique_ptr<ptv::pdb_type_descriptor>&& descriptor
        ) noexcept;

    private:
        void SetupModelData(
            const std::vector<std::unique_ptr<ptv::pdb_abstract_type_member>>& members,
            TypeDescriptorElement* parent
        ) noexcept;

    public: // interface QAbstractItemModel
        QVariant data(
            const QModelIndex& index,
            int role
        ) const override;

        Qt::ItemFlags flags(
            const QModelIndex& index
        ) const override;

        QVariant headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole
        ) const override;

        QModelIndex index(
            int row,
            int column,
            const QModelIndex& parent = {}
        ) const override;

        QModelIndex parent(
            const QModelIndex& index
        ) const override;

        int rowCount(
            const QModelIndex& parent = {}
        ) const override;

        int columnCount(
            const QModelIndex& parent = {}
        ) const override;
    };
}
